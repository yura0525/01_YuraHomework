#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>

OVERLAPPED ov;
LARGE_INTEGER g_size;
char* g_buf = NULL;

//�����ڵ忡�� ��Ƽ����Ʈ�� �ٲٴ� �Լ�.
char* GetW2M(WCHAR* data)
{
	char cData[3333] = { 0, };

	//��ȯ�� ũ�⸦ ã�´�.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);

	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, cData, iLength, NULL, NULL);
	return cData;
}

//��Ƽ����Ʈ���� �����ڵ�� �ٲٴ� �Լ�.
WCHAR* GetM2W(char* data)
{
	WCHAR cData[3333] = { 0, };

	//��ȯ�� ũ�⸦ ã�´�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);

	int iRet = MultiByteToWideChar(CP_ACP, 0, data, -1, cData, iLength);
	return cData;
}

std::string w2m(std::wstring data)
{
	std::string ret = std::string(data.begin(), data.end());
	return ret;
}

std::wstring m2w(std::string data)
{
	std::wstring ret = std::wstring(data.begin(), data.end());
	return ret;
}


//4096 * 1024 ��ŭ �ɰ��� �д´�.
//������ �ѹ��� ������ �δ㽺���� ������ �ɰ��� �д´�.
void FileRead(HANDLE hReadFile, DWORD offset)
{
	DWORD dwLength = 4096 * 1024;
	g_size.QuadPart += offset;

	ov.Offset = g_size.LowPart;
	ov.OffsetHigh = g_size.HighPart;
	char* pTemp = &g_buf[g_size.QuadPart];

	DWORD dwRead;
	bool iRet = ReadFile(hReadFile, pTemp, dwLength, &dwRead, &ov);

	//�񵿱�������Լ�. OVERLAPPED�� ����ߴ�.
	//�׷��� �бⰡ ������ ���� ���ϵǾ iRet == FALSE�̹Ƿ�
	//GetLastError() == ERROR_IO_PENDING���� �а� �ִ������� Ȯ���ؾ��Ѵ�.
	//�бⰡ �� �������� ������ ������Ѵ�.
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

	HANDLE hReadFile = CreateFileA("../../TBasisSample3D.zip", GENERIC_READ,
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

	CloseHandle(hReadFile);

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
			DWORD dwRet = WaitForSingleObject(hWriteFile, 0);

			if (dwRet == WAIT_OBJECT_0)
			{
				printf("\n WAIT_OBJECT_0 %s : %ld", "FinishWrite....", ov.InternalHigh);
				break;
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				DWORD trans;

				bool ret = GetOverlappedResult(hWriteFile, &ov, &trans, FALSE);
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

	printf("���� ���� �Ϸ�");
	delete[] g_buf;

	CloseHandle(hWriteFile);
}