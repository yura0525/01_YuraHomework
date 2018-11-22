#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>

//유니코드에서 멀티	바이트로 바꾸는 함수
char* GetW2M(const WCHAR* data)
{
	char cData[3333] = { 0, };

	//변환된 크기를 찾는다.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, cData, iLength, NULL, NULL);

	return cData;
}

//멀티바이트에서 유니코드로 바꾸는 함수
WCHAR* GetM2W(const char* data)
{
	WCHAR cData[3333] = { 0, };

	//변환된 크기를 찾는다.
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

void main()
{
	HANDLE hWriteFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hWriteFile != NULL)
	{
		TCHAR buf[] = L"1234567890";
		DWORD dwLength = sizeof(buf);
		DWORD dwWritten;
		bool iRet = WriteFile(hWriteFile, buf, dwLength, &dwWritten, NULL);
		if (iRet == TRUE)
		{
			printf("출력 완료\n");
		}
	}
	CloseHandle(hWriteFile);

	HANDLE hReadFile = CreateFile(L"test.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hReadFile != NULL)
	{
		TCHAR buf[1024] = { 0, };
		DWORD dwLength = sizeof(buf);
		DWORD dwRead;
		bool iRet = ReadFile(hReadFile, buf, dwLength, &dwRead, NULL);

		if (iRet == TRUE)
		{
			std::wstring strWData = buf;
			std::string strCData = w2m(strWData);

			printf("\n%s", strCData.c_str());
			_tcprintf(L"\n%s", m2w(strCData.c_str()).c_str());

			printf("\n%s", GetW2M(buf));
			_tcprintf(L"\n%s", GetM2W(strCData.c_str()));
		}
	}
	CloseHandle(hReadFile);
}
