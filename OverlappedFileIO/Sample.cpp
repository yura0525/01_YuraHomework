#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>

OVERLAPPED ov;
LARGE_INTEGER g_size;
char* g_buf = NULL;

//4096만큼 쪼개서 읽는다.
//파일을 한번에 읽으면 부담스럽기 때문에 쪼개서 읽는다.
void FileRead(HANDLE hReadFile, DWORD offset)
{
	DWORD dwLength = 4096;
	g_size.QuadPart += offset;

	ov.Offset = g_size.LowPart;
	ov.OffsetHigh = g_size.HighPart;
	char* pTemp = &g_buf[g_size.QuadPart];

	DWORD dwRead;
	bool iRet = ReadFile(hReadFile, pTemp, dwLength, &dwRead, &ov);

	//비동기입출력함수. OVERLAPPED을 사용했다.
	//그래서 읽기가 끝나기 전에 리턴되어서 iRet == FALSE이므로
	//GetLastError() == ERROR_IO_PENDING으로 읽고 있는중인지 확인해야한다.
	//읽기가 다 끝났을때 통지를 해줘야한다.
	if (iRet == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			printf("\n%s", "ERROR_IO_PENDING Read...");
		}
	 }
}

void main()
{
	LARGE_INTEGER fileSize;
	ZeroMemory(&ov, sizeof(OVERLAPPED));

	HANDLE hReadFile = CreateFileA("../../xCoreLib_2.7z", GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &fileSize);

		g_buf = new char[fileSize.QuadPart];
		if (g_buf == NULL)
			return;

		FileRead(hReadFile, 0);
	}

	while (1)
	{
		DWORD trans = 0;
		bool ret = GetOverlappedResult(hReadFile, &ov, &trans, TRUE);
		if (ret == TRUE)
		{
			if (fileSize.QuadPart <= g_size.QuadPart)
				break;

			FileRead(hReadFile, trans);
		}
		else
		{
			printf("\n%s", "READ...");
		}
	}

	printf("\n%s : %ld", "FinishRead...", g_size.QuadPart);

	OVERLAPPED wov;
	ZeroMemory(&wov, sizeof(OVERLAPPED));

	HANDLE hWriteFile = CreateFileA("data_2.zip", GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		bool iRet = WriteFile(hWriteFile, g_buf, fileSize.QuadPart, &dwWritten, &wov);

		if (iRet == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n%s", "ERROR_IO_PENDING Write...");
			}
		}

		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hWriteFile, INFINITE);

			if (dwRet == WAIT_OBJECT_0)
			{
				printf("\n WAIT_OBJECT_0 %s : %ld", "FinishWrite....", wov.InternalHigh);
				break;
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				DWORD trans;

				bool ret = GetOverlappedResult(hWriteFile, &wov, &trans, TRUE);
				if (ret == TRUE)
				{
					bool iRet = WriteFile(hWriteFile, g_buf, fileSize.QuadPart, &dwWritten, &wov);

					printf("\n WAIT_TIMEOUT %s : %ld", "FinishWrite...", wov.InternalHigh);
					break;
				}
				else
				{
					static LONG dwSum = 0;
					dwSum += wov.InternalHigh;
					printf("\n%s : %ld : %ld", "Write....", wov.InternalHigh, dwSum);
				}
			}
		}
	}

	printf("파일 복사 완료");
	delete[] g_buf;

	CloseHandle(hReadFile);
	CloseHandle(hWriteFile);
}