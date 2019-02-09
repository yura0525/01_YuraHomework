#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

// �񵿱� �۾� ��=FALSE, �۾��Ϸ�=TRUE
#define HasOverlappedIoCompleted(pOverlapped) ((pOverlapped)->Internal != STATUS_PENDING)

//�񵿱� �Է�
OVERLAPPED g_ReadOV;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;

CHAR* g_pmbData;
CHAR* g_pmbOffsetData;
CHAR* g_pmbdataCopy;

//dwStepSize ������ ������ ���������� �ε��Ѵ�.
DWORD FileRead(HANDLE hHandle, DWORD offset)
{
	g_LargeRead.QuadPart += offset;
	g_ReadOV.Offset = g_LargeRead.LowPart;
	g_ReadOV.OffsetHigh = g_LargeRead.HighPart;
	g_pmbOffsetData = &g_pmbData[g_LargeRead.LowPart];

	DWORD dwStepSize = 4096 * 1024;
	DWORD dwBytesRead;
	DWORD ret = ReadFile(hHandle, g_pmbOffsetData, dwStepSize, &dwBytesRead, &g_ReadOV);
	if (ret == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//printf("\n�۾� ��!");
		}
	}
	else
	{
		printf("\n�Է� �Ϸ�!");
		CloseHandle(hHandle);
		delete[] g_pmbData;
		return 0;
	}
	return 1;
}

DWORD Load(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);

	DWORD cTotalBytes;
	LARGE_INTEGER FileSize;
	HANDLE hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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
	HANDLE hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

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
		//OVERLAPPED readOV;
		ZeroMemory(&g_ReadOV, sizeof(OVERLAPPED));
		g_ReadOV.hEvent = NULL;

		FileRead(hHandle, 0);

		static int iCnt = 0;

		// �񵿱� ������� �۾��ϼ�
		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hHandle, 0);

			if (dwRet == WAIT_OBJECT_0)
			{
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &g_ReadOV, &Trans, TRUE);

				if (Result == TRUE)
				{
					if (g_LargeRead.LowPart >= cTotalBytes)
					{
						printf("\n--->%ld �б� �Ϸ�!!!!---->", g_LargeRead.QuadPart);
						double av = (double)g_LargeRead.QuadPart /
							(double)cTotalBytes * 100.0f;
						//system("cls");
						printf("\n[���� ũ��: %ld KB", cTotalBytes / 1024);
						printf("--->[%10.4lf%%]:%d KB!",
							av, g_LargeRead.QuadPart / 1024);
						break;
					}
					printf("\n%d �д� ��[:%lu]", iCnt++, g_LargeRead.QuadPart);
					FileRead(hHandle, Trans);
				}
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &g_ReadOV, &Trans, FALSE);
				if (Result == FALSE)
				{
					if (GetLastError() == ERROR_IO_INCOMPLETE)
					{

						printf("\n�д� ��[:%d]", iCnt++);
					}
				}
				else
				{
					float av = (float)g_LargeRead.LowPart /
						(float)cTotalBytes * 100.0f;
					//system("cls");
					printf("\n[���� ũ��: %13ld KB", cTotalBytes / 1024);
					printf("\n--->[%10.4f%%]:%d KB!",
						av, g_LargeRead.LowPart / 1024);
				}
			}
			else
			{
				printf("\n--->%d Error!!!!", dwRet);
				break;
			}
			//Sleep(1);
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
	ov.hEvent = NULL;

	//�񵿱� ���
	HANDLE hWriteFile = CreateFile(L"TBasis2Dcopy.zip", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

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
			BOOL Result = GetOverlappedResult(hWriteFile, &ov, &Trans, TRUE);
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