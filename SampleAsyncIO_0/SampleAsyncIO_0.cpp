#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// �񵿱� �۾� ��=FALSE, �۾��Ϸ�=TRUE
#define HasOverlappedIoCompleted(pOverlapped) ((pOverlapped)->Internal != STATUS_PENDING)

CHAR* g_pmbData;
CHAR* g_pmbDataCopy;

DWORD Load(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);

	DWORD cTotalBytes;
	LARGE_INTEGER FileSize;
	HANDLE hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hHandle != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hHandle, &FileSize);
		cTotalBytes = FileSize.LowPart;

		g_pmbData = new char[cTotalBytes];
		if (!g_pmbData)
		{
			CloseHandle(hHandle);
			return 0;
		}

		DWORD dwBytesRead;
		if (!ReadFile(hHandle, g_pmbData, cTotalBytes, &dwBytesRead, NULL))
		{
			CloseHandle(hHandle);
			delete[] g_pmbData;
			return 0;
		}
		SetFilePointer(hHandle, 0, 0, FILE_BEGIN);
	}
	else
	{
		return 0;
	}
	CloseHandle(hHandle);
	return cTotalBytes;
}

DWORD AsyncLoad(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);

	DWORD cTotalBytes;
	LARGE_INTEGER FileSize;
	HANDLE hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL| FILE_FLAG_OVERLAPPED, NULL);

	if (hHandle != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hHandle, &FileSize);
		cTotalBytes = FileSize.LowPart;
		printf("\n[���� ũ��: %ld KB", cTotalBytes / 1024);

		g_pmbData = new char[cTotalBytes];
		if (!g_pmbData)
		{
			CloseHandle(hHandle);
			return 0;
		}

		// �񵿱� �Է�
		OVERLAPPED readOV;
		ZeroMemory(&readOV, sizeof(OVERLAPPED));

		DWORD dwBytesRead;
		DWORD ret = ReadFile(hHandle, g_pmbData, cTotalBytes, &dwBytesRead, &readOV);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n �б� �۾� ��!");
			}
		}
		if (ret == TRUE)
		{
			printf("\n�Է� �Ϸ�!");
			CloseHandle(hHandle);
			delete[] g_pmbData;
			return 0;
		}

		// �񵿱� ������� �۾��ϼ�
		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hHandle, 0);

			if (dwRet == WAIT_OBJECT_0)
			{
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &readOV, &Trans, FALSE);

				if (Trans >= cTotalBytes)
				{
					printf("\n--->%ld �б� �Ϸ�!!!!", readOV.InternalHigh);
					break;
				}
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				/*1. bWait == TRUE && OVERLAPPED.internal == STATUS_PENDING
				bWait�� TRUE�� ��쿡 �̸� COMMTIMEOUTS�� ������ Ÿ�Ӿƿ� ��
				��ŭ ��ٸ���.��ٸ��� ���� �Ϸ�Ǹ� TRUE ��ȯ TIMEOUT�Ǹ� FALSE ��ȯ
				GetLastError() �Լ��� ERROR_IO_INCOMPLETE ��ȯ
				2. bWait == FALSE && OVERLAPPED.internal == STATUS_PENDING
				bWait�� FALSE�� ��쿡 �Ϸ�Ǳ� ��ٸ��� �ʰ� �ٷ� ������ ����Ʈ�� �ְ� ��ȯ�ȴ�.
				���� �����۾��� ������ �ʾҴٸ� FALSE�� ��ȯ�ϰ� GetLastError() �Լ���
				ERROR_IO_INCOMPLETE ��ȯ*/

				static int iCnt = 0;
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &readOV, &Trans, FALSE);
				if (Result == FALSE)
				{
					if (GetLastError() == ERROR_IO_INCOMPLETE)
					{
						printf("\n�д� ��[:%d]", iCnt++);
					}
				}
				else
				{
					printf("\n--->%d �б� �Ϸ�.", readOV.InternalHigh);
				}
			}
			else
			{
				printf("\n--->%d Error!!!!", dwRet);
				break;
			}
			Sleep(1);
		}
	}
	else
	{
		return 0;
	}

	SetFilePointer(hHandle, 0, 0, FILE_BEGIN);
	CloseHandle(hHandle);
	return cTotalBytes;
}

int main()
{
	// ����  �Է����� ���� �ε�
	//DWORD dwFileSize = Load(L"../../data/004.flac");
	// �񵿱� �Է����� ���� �ε�
	DWORD dwFileSize = AsyncLoad(L"../../data/004.flac");

	if (dwFileSize == 0)
		return 0;
	printf("\n���� ���� �ε� �Ϸ�!");

	OVERLAPPED ov;
	ZeroMemory(&ov, sizeof(OVERLAPPED));
	ov.hEvent = 0;

	//�񵿱� ���
	HANDLE hWriteFile = CreateFile(L"004.flac", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	printf("\n�񵿱� ��� ����----------->!");
	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		BOOL ret = WriteFile(hWriteFile, g_pmbData, dwFileSize, &dwWritten, &ov);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n�۾� ��!");
			}
		}
		else
		{
			printf("\n��� �Ϸ�!");
		}
	}

	// �񵿱� ������� �۾��ϼ�
	while (1)
	{
		DWORD dwRet = WaitForSingleObject(hWriteFile, 0);
		if (dwRet == WAIT_OBJECT_0)
		{
			DWORD Trans = 0;
			BOOL Result = GetOverlappedResult(hWriteFile, &ov, &Trans, FALSE);
			if (Trans >= dwFileSize)
			{
				delete[] g_pmbData;
				CloseHandle(hWriteFile);
				printf("\n--->%d ���� ��¿Ϸ�!!!!", ov.InternalHigh);
				break;
			}
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			static int iCnt = 0;
			DWORD Trans = 0;
			BOOL Result = GetOverlappedResult(hWriteFile, &ov, &Trans, FALSE);
			if (Result == FALSE)
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					printf("\n�����[:%d]", iCnt++);
				}
			}
			else
			{
				printf("\n--->%d ���� ��¿Ϸ�!!!!", ov.InternalHigh);
			}
		}
		else
		{
			delete[] g_pmbData;
			CloseHandle(hWriteFile);
			printf("\n--->%d Error!!!!", dwRet);
			break;
		}
		Sleep(1);
	}
	CloseHandle(ov.hEvent);
	printf("\n ���α׷� ����");
	getchar();
	return 0;
}