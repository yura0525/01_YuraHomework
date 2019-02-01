#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")

HANDLE g_hEvent;
SOCKET sock;

DWORD WINAPI ConnectThread(LPVOID arg)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;//sockaddr name;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);// host byte ���� to network byte ���� short
	sa.sin_addr.s_addr = inet_addr("192.168.0.2");
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 1; }
	SetEvent(g_hEvent);
	//ResetEvent(g_hEvent);
	return 0;
}

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		fgets(buf, 256, stdin);
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		if (strlen(buf) == 0 ||
			_stricmp(buf, "quit") == 0)
		{
			break; // ���ʹ� ����!
		}
		int iSendByte = send(sock, buf, strlen(buf), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d ����Ʈ�� �����Ͽ����ϴ�.", iSendByte);
	}
	closesocket(sock);
	return 0;
}
DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	printf("\n���� ������ �Է��Ͻÿ�?");
	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		int iLen = 0;
		int iRecvByte = recv(sock, buf, 256, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("[��������]: %s", "���� ������ ���Ͽ� ����Ǿ���.");
			break;
		}
		buf[iRecvByte] = 0;
		printf("\n[�����޼���]: %s", buf);
	}
	closesocket(sock);
	return 0;
}
int main()
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceClinetMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		MessageBox(NULL, L"�̹� �ٸ� �ν��Ͻ��� ���� ���Դϴ�.", L"�˸�", MB_OK);
		return 1;
	}

	// 2 : FALSE(�ڵ����� �̺�Ʈ),TRUE(�������� �̺�Ʈ)
	// 3 : �ʱⰪ(FALSE: ��ñ׳�, TRUE:�ñ׳�)
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvent");
	//ResetEvent(g_hEvent);

	HANDLE hSendThread, hRecvThread, hConnectThread;
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		// ���� ����
		DWORD dwConnectThreadID;
		hConnectThread = CreateThread(0, 0,
			ConnectThread,
			(LPVOID)0,
			0,
			&dwConnectThreadID);

		//WaitForSingleObject(hConnectThread, INFINITE);
		WaitForSingleObject(g_hEvent, INFINITE);
		//while (g_bConnect == false);
		//ResetEvent(g_hEvent);

		{
			// ���� ����
			DWORD dwSendThreadID;

			hSendThread = CreateThread(0, 0,
				SendThread,
				(LPVOID)sock,
				0,
				&dwSendThreadID);
			DWORD dwRecvThreadID;
			hRecvThread = CreateThread(0, 0,
				RecvThread,
				(LPVOID)sock,
				0,
				&dwRecvThreadID);
		}

	}
	// �����尡 ����Ǿ����� Ȯ��
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);

	// ������ ���� �Ҹ�
	iRet = WSACleanup();
	CloseHandle(hExecuteMutex);
	CloseHandle(g_hEvent);
	return iRet;
}