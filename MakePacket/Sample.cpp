#include "Sample.h"

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	TUser* pUser = &g_allClientSocket[sock];

	char buffer[256] = { 0, };
	int recvbyte = 0;
	int iRet = 0;
	while (1)
	{
		iRet = recv(sock, &buffer[recvbyte], PACKET_HEADER_SIZE - recvbyte, 0);
		if (CheckReturn(iRet) <= 0)
			break;

		recvbyte += iRet;
		if (recvbyte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			memcpy(&packet.ph, buffer, PACKET_HEADER_SIZE);
			
			int rByte = 0;
			int iRecvByte = 0;
			do
			{
				rByte = recv(sock, (char*)&packet.msg[iRecvByte], packet.ph.len - iRecvByte, 0);
				if (CheckReturn(iRet) <= 0)
					break;

				iRecvByte += rByte;
			} while (packet.ph.len > iRecvByte);

			if (CheckReturn(iRet) <= 0)
				break;

			recvbyte = 0;
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				cout << "��Ŷ �ϼ�:--->" << packet.msg;
				Broadcastting(&packet);
			}
			break;
			}
		}
		Sleep(1);
	}
	DeleteUser(sock);
	return 0;
}
// ���� ����
// ������, TCP��������(SOCK_STREAM,IPPROTO_TCP)
// �񿬰���, UDP ��������(SOCK_DGRAM,IPPROTO_UDP)
void main()
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return;
	}
	InitializeCriticalSection(&g_crit);
	// TRUE =  ������ �����尡 ������
	// FALSE = �����ڰ� ����.(��ȣ����) : ���ȣ����
	g_hMutex = CreateMutex(NULL, FALSE, L"CHAT_MUTEX");

	unsigned short iPort = 10000;
	WSADATA wsa;
	int iRet;
	HANDLE hThread = NULL;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iPort);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	// ������ ��� ip�� �����ض�.
	iRet = bind(listenSock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
		return;

	iRet = listen(listenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
		return;

	SOCKADDR_IN  clientaddr;
	SOCKET client;
	while (1)
	{
		int addlen = sizeof(clientaddr);
		client = accept(listenSock, (SOCKADDR*)&clientaddr, &addlen);
		if (client == INVALID_SOCKET)
			break;

		TUser user(client, clientaddr);
		WaitForSingleObject(g_hMutex, INFINITE);
		GreetMessage(user);
		ReleaseMutex(g_hMutex);

		DWORD dwRecvThreadID;
		hThread = CreateThread(0, 0, ClientThread, (LPVOID)client, 0, &dwRecvThreadID);
	}

	closesocket(listenSock);

	iRet = WSACleanup();

	DeleteCriticalSection(&g_crit);
	CloseHandle(hThread);
	CloseHandle(g_hMutex);
	CloseHandle(hExecuteMutex);
	return;
}