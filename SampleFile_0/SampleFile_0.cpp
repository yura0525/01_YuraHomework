#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0, };

	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, 
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}

int main()
{
	HANDLE hWriteFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hWriteFile != NULL)
	{
		TCHAR buf[] = L"KGCA";
		DWORD dwWritten;
		DWORD dwLength = sizeof(buf);

		BOOL ret = WriteFile(hWriteFile, buf, dwLength, &dwWritten, NULL);
		if (ret == TRUE)
		{
			printf("��� �Ϸ�!");
		}
	}
	CloseHandle(hWriteFile);

	HANDLE hReadFile = CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hReadFile != NULL)
	{
		TCHAR buf[1024];
		DWORD dwRead;
		DWORD dwLength = sizeof(buf);
		BOOL ret = ReadFile(hReadFile, buf, dwLength, &dwRead, NULL);
		if (ret == TRUE)
		{
			_tprintf(L"\n�б� �Ϸ�! : %s", buf);
			_tcprintf(L"\n�б� �Ϸ�! : %s", buf);
			printf("\n--->%s", GetWtM(buf));
		}
	}
	CloseHandle(hReadFile);
	return 0;
}