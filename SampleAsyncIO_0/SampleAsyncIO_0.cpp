#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 비동기 작업 중=FALSE, 작업완료=TRUE
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
		printf("\n[파일 크기: %ld KB", cTotalBytes / 1024);

		g_pmbData = new char[cTotalBytes];
		if (!g_pmbData)
		{
			CloseHandle(hHandle);
			return 0;
		}

		// 비동기 입력
		OVERLAPPED readOV;
		ZeroMemory(&readOV, sizeof(OVERLAPPED));

		DWORD dwBytesRead;
		DWORD ret = ReadFile(hHandle, g_pmbData, cTotalBytes, &dwBytesRead, &readOV);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n 읽기 작업 중!");
			}
		}
		if (ret == TRUE)
		{
			printf("\n입력 완료!");
			CloseHandle(hHandle);
			delete[] g_pmbData;
			return 0;
		}

		// 비동기 입출력의 작업완성
		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hHandle, 0);

			if (dwRet == WAIT_OBJECT_0)
			{
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &readOV, &Trans, FALSE);

				if (Trans >= cTotalBytes)
				{
					printf("\n--->%ld 읽기 완료!!!!", readOV.InternalHigh);
					break;
				}
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				/*1. bWait == TRUE && OVERLAPPED.internal == STATUS_PENDING
				bWait가 TRUE인 경우에 미리 COMMTIMEOUTS에 설정한 타임아웃 값
				만큼 기다린다.기다리는 도중 완료되면 TRUE 반환 TIMEOUT되면 FALSE 반환
				GetLastError() 함수는 ERROR_IO_INCOMPLETE 반환
				2. bWait == FALSE && OVERLAPPED.internal == STATUS_PENDING
				bWait가 FALSE인 경우에 완료되길 기다리지 않고 바로 쓰기한 바이트를 넣고 반환된다.
				아직 쓰기작업이 끝나지 않았다면 FALSE를 반환하고 GetLastError() 함수는
				ERROR_IO_INCOMPLETE 반환*/

				static int iCnt = 0;
				DWORD Trans = 0;
				BOOL Result = GetOverlappedResult(hHandle, &readOV, &Trans, FALSE);
				if (Result == FALSE)
				{
					if (GetLastError() == ERROR_IO_INCOMPLETE)
					{
						printf("\n읽는 중[:%d]", iCnt++);
					}
				}
				else
				{
					printf("\n--->%d 읽기 완료.", readOV.InternalHigh);
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
	// 동기  입력으로 파일 로드
	//DWORD dwFileSize = Load(L"../../data/004.flac");
	// 비동기 입력으로 파일 로드
	DWORD dwFileSize = AsyncLoad(L"../../data/004.flac");

	if (dwFileSize == 0)
		return 0;
	printf("\n원본 파일 로딩 완료!");

	OVERLAPPED ov;
	ZeroMemory(&ov, sizeof(OVERLAPPED));
	ov.hEvent = 0;

	//비동기 출력
	HANDLE hWriteFile = CreateFile(L"004.flac", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

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
			BOOL Result = GetOverlappedResult(hWriteFile, &ov, &Trans, FALSE);
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