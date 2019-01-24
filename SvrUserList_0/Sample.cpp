#define  _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include "TUser.h"

#pragma comment(lib, "ws2_32.lib")


CRITICAL_SECTION g_crit;
std::list<TUser*> g_allClientList;
TPacketPool			g_PacketList;
int g_iNumClient = 0;

int SendMsg(TUser* pUser, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));

	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = send(pUser->m_Socket, (char*)&sendmsg, sendmsg.ph.len, 0);
	return iRet;
}

DWORD WINAPI WorkThread(LPVOID arg)
{
	while (1)
	{
		//��ε�ĳ����
		EnterCriticalSection(&g_crit);
		{
			std::list<UPACKET>::iterator iterPacket;
			for (iterPacket = I_Pool.m_RecvPacketList.begin();
				iterPacket != I_Pool.m_RecvPacketList.end();
				iterPacket++)
			{
				UPACKET* pPacket = &(UPACKET)*iterPacket;
				printf("\n[����������]:%s", pPacket->msg);

				std::list<TUser*>::iterator  iter;
				for (iter = g_allClientList.begin();
					iter != g_allClientList.end();
					iter++)
				{
					TUser* pSendUser = (TUser*)*iter;
					//int iSendByte = SendMsg(pSendUser, buf, PACKET_CHAT_MSG);
					int iSendByte = send(pSendUser->m_Socket, (char*)pPacket,
						pPacket->ph.len, 0);

					if (iSendByte == SOCKET_ERROR)
					{
						printf("���� ����\n");
						break; // Ŭ���̾�Ʈ ����
					}
				}
			}
		}
		LeaveCriticalSection(&g_crit);
		I_Pool.m_RecvPacketList.clear();
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	TUser* pUser = (TUser*)arg;
	char buf[1024] = { 0, };
	SOCKADDR_IN clientaddr;
	while (1)
	{
		int iRecvByte = recv(pUser->m_Socket, buf, 1024, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("Ŭ���̾�Ʈ ���� ����\n");
			printf("Ŭ���̾�Ʈ ���� ���� : IP:%s, PORT:%d\n",
				pUser->m_szIP.c_str(), pUser->m_dwPort);
			break; // Ŭ���̾�Ʈ ����
		}
		pUser->m_RecvStreamPacket.Put(buf, iRecvByte, pUser);
		Sleep(1);
	}
	// Ŭ���̾�Ʈ ���� ó��
	EnterCriticalSection(&g_crit);
	std::list<TUser*>::iterator  iter;
	for (iter = g_allClientList.begin();
		iter != g_allClientList.end();
		iter++)
	{
		TUser* pDelUser = (TUser*)*iter;
		if (pDelUser->m_Socket == pUser->m_Socket)
		{
			closesocket(pDelUser->m_Socket);
			delete pDelUser;
			g_allClientList.erase(iter);
			break;
		}
	}
	g_iNumClient = g_allClientList.size();
	LeaveCriticalSection(&g_crit);
	return 0;
}

void GreetMessage(SOCKET sock, SOCKADDR_IN clientaddr)
{
	TUser* pUser = new TUser;
	pUser->m_Socket = sock;
	pUser->m_UserAddress = clientaddr;
	pUser->m_szIP = inet_ntoa(clientaddr.sin_addr);
	pUser->m_dwPort = ntohs(clientaddr.sin_port);
	g_allClientList.push_back(pUser);

	DWORD dwRecvThreadID;
	pUser->m_hThread = CreateThread(0, 0, ClientThread, (LPVOID)pUser, 0, &pUser->m_dwRecvThreadID);

	printf("Ŭ���̾�Ʈ ���� : IP:%s, PORT:%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[����]ȯ���մϴ�.\n");
	SendMsg(pUser, message, PACKET_CHAT_MSG);
	g_iNumClient = g_allClientList.size();
}

void main()
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return;
	}
	InitializeCriticalSection(&g_crit);

	unsigned short iPort = 10000;// atoi(argv[1]);
								 // ������ ����(����) �ʱ�ȭ
	WSADATA wsa;
	int iRet;
	HANDLE hThread;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	{
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
		{
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

			//��Ŷ Ǯ �۾� ������ ����
			DWORD dwRecvThreadID;
			hThread = CreateThread(0, 0, WorkThread, (LPVOID)0, 0, &dwRecvThreadID);

			while (1)
			{
				int addlen = sizeof(clientaddr);
				client = accept(listenSock, (SOCKADDR*)&clientaddr, &addlen);

				//��ȣ������ ��쿡�� ����
				EnterCriticalSection(&g_crit);
				GreetMessage(client, clientaddr);
				LeaveCriticalSection(&g_crit);
			}
		}
		closesocket(listenSock);
	}
	// ������ ���� �Ҹ�
	iRet = WSACleanup();
	DeleteCriticalSection(&g_crit);
	CloseHandle(hExecuteMutex);
	g_allClientList.clear();
	return;
}