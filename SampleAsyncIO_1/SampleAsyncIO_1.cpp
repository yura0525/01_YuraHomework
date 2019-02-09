#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

// 비동기 작업 중=FALSE, 작업완료=TRUE
#define HasOverlappedIoCompleted(pOverlapped) ((pOverlapped)->Internal != STATUS_PENDING)

//비동기 입력
OVERLAPPED g_ReadOV;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;

CHAR* g_pmbData;
CHAR* g_pmbOffsetData;
CHAR* g_pmbdataCopy;

//dwStepSize 사이즈 단위로 연속적으로 로딩한다.
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
			//printf("\n작업 중!");
		}
	}
	else
	{
		printf("\n입력 완료!");
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
		printf("\n[파일 크기: %ld KB", cTotalBytes / 1024);

		g_pmbData = new char[cTotalBytes];
		if (!g_pmbData)
		{
			CloseHandle(hHandle);
			return 0;
		}

		// 비동기 입력
		//OVERLAPPED readOV;
		ZeroMemory(&g_ReadOV, sizeof(OVERLAPPED));
		g_ReadOV.hEvent = NULL;

		FileRead(hHandle, 0);

		static int iCnt = 0;

		// 비동기 입출력의 작업완성
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
						printf("\n--->%ld 읽기 완료!!!!---->", g_LargeRead.QuadPart);
						double av = (double)g_LargeRead.QuadPart /
							(double)cTotalBytes * 100.0f;
						//system("cls");
						printf("\n[파일 크기: %ld KB", cTotalBytes / 1024);
						printf("--->[%10.4lf%%]:%d KB!",
							av, g_LargeRead.QuadPart / 1024);
						break;
					}
					printf("\n%d 읽는 중[:%lu]", iCnt++, g_LargeRead.QuadPart);
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

						printf("\n읽는 중[:%d]", iCnt++);
					}
				}
				else
				{
					float av = (float)g_LargeRead.LowPart /
						(float)cTotalBytes * 100.0f;
					//system("cls");
					printf("\n[파일 크기: %13ld KB", cTotalBytes / 1024);
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
	// 동기  입력으로 파일 로드
	//DWORD dwFileSize = Load(L"../../data/004.flac");
	// 비동기 입력으로 파일 로드
	DWORD dwFileSize = AsyncLoad(L"../../data/004.flac");

	if (dwFileSize == 0)
		return 0;
	printf("\n원본 파일 로딩 완료!");

	OVERLAPPED ov;
	ZeroMemory(&ov, sizeof(OVERLAPPED));
	ov.hEvent = NULL;

	//비동기 출력
	HANDLE hWriteFile = CreateFile(L"TBasis2Dcopy.zip", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	printf("\n비동기 출력 시작----------->!");
	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		BOOL ret = WriteFile(hWriteFile, g_pmbData, dwFileSize, &dwWritten, &ov);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n작업 중!");
			}
		}
		else
		{
			printf("\n출력 완료!");
		}
	}

	// 비동기 입출력의 작업완성
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
				printf("\n--->%d 복사 출력완료!!!!", ov.InternalHigh);
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
					printf("\n출력중[:%d]", iCnt++);
				}
			}
			else
			{
				printf("\n--->%d 복사 출력완료!!!!", ov.InternalHigh);
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
	printf("\n 프로그램 종료");
	getchar();
	return 0;
}