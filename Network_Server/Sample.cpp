#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <map>
#include "Sample.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

std::map<SOCKET, TUser> g_allUser;
typedef std::map<SOCKET, TUser>::iterator ITOR;

CRITICAL_SECTION g_Crit;
HANDLE			g_hMutex;

void AddUser(TUser& user)
{
	//EnterCriticalSection(&g_Crit);
	//EnterCriticalSection�� ����. ������ ��ٷ��ش�.
	//���Ѵ���ϸ� WAIT_TIMEOUT�� �ʿ䰡 ����. �ð��� �־��ٶ� WAIT_TIMEOUT�� ����.
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);

	if (dwRet != WAIT_TIMEOUT)
	{
		g_allUser.insert(make_pair(user.sock, user));
		//g_allUser[g_iNumClient] = user;
		//g_iNumClient++;
	}

	//LeaveCriticalSection(&g_Crit);
	ReleaseMutex(g_hMutex);
}

int Broadcastting(char* pMsg)
{
	//EnterCriticalSection(&g_Crit);
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);

	//�߿�!!!:���������ڰ� ����. if�� �ȿ��� �����Ѵ�.
	for (ITOR itor = g_allUser.begin(); itor != g_allUser.end(); )
	{
		TUser* pUser = (TUser*)&(itor->second);
		if (pUser != NULL)
		{
			if (SendMsg(pUser->sock, pMsg, PACKET_CHAT_MSG) <= 0)
			{
				closesocket(pUser->sock);
				g_allUser.erase(itor++);
			}
			else
			{
				itor++;
			}
		}
		
	}

	ReleaseMutex(g_hMutex);
	//LeaveCriticalSection(&g_Crit);
	return 1;
}

void tMSG(TUser* pUser, const char* msg)
{
	printf("\n ip = %s, msg = %s", inet_ntoa(pUser->clientAddr.sin_addr), msg);
}

void DelUser(TUser* pUser)
{
	tMSG(pUser, " �����Ͽ����ϴ�.\n");

	//EnterCriticalSection(&g_Crit);
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
	{
		ITOR itor = g_allUser.find(pUser->sock);
		closesocket(pUser->sock);

		if (itor != g_allUser.end())
		{
			g_allUser.erase(itor);
		}
	}

	//for (int iUser = 0; iUser < g_iNumClient; iUser++)
	//{
	//	if (g_allUser[iUser].sock == pUser->sock)
	//	{
	//		//���ﶧ �ڿ��� �ϳ��� ���ܿ��� g_iNumClient�� �ϳ� ���δ�.
	//		for (int iDel = iUser; iDel < g_iNumClient; iDel++)
	//		{
	//			g_allUser[iDel] = g_allUser[iDel + 1];
	//		}
	//		break;
	//	}
	//}
	//g_iNumClient--;

	T_ERROR();
	ReleaseMutex(g_hMutex);
	//LeaveCriticalSection(&g_Crit);
	return;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	TUser* pUser = (TUser*)arg;
	SOCKET sock = pUser->sock;

	char buffer[256] = { 0, };
	int recvByte = 0;
	int iRet = 0;
	bool bConnect = true;

	while (1)
	{
		ZeroMemory(&buffer, sizeof(char) * 256);

		//ó���� PACKET_HEADER_SIZE�����ŭ �޴´�.
		iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
		if (iRet == 0 ||iRet == SOCKET_ERROR)
		{
			break;
		}

		recvByte += iRet;

		//��Ŷ�� PACKET_HEADER_SIZE��ŭ �޾�����.
		if (recvByte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(UPACKET));
			memcpy(&packet.ph, buffer, sizeof(char) * PACKET_HEADER_SIZE);

			//��Ŷ�� packet.ph.len���̸�ŭ �޴´�. ��Ŷ�� �ٹ޾ƾ� while�� ����.
			//packet.ph.len���� msg�� ���̸� ����ִ�. PACKET_HEADER_SIZE ���������� �ʴ�.
			int rByte = 0;
			while (rByte < packet.ph.len)
			{
				
				int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char) * (packet.ph.len - rByte), 0);

				if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				rByte += iRecvByte;
			} 

			recvByte = 0;
			if (bConnect)
			{
				switch (packet.ph.type)
				{
					case PACKET_CHAT_MSG:
					{
						printf("\n��Ŷ �ϼ� %s\n", packet.msg);
						Broadcastting(packet.msg);
					}
					break;
					case PACKET_CREATE_CHARACTER:
					{
						CHARACTER_INFO cInfo;
						memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
						printf("\n��Ŷ �ϼ� %s\n", packet.msg);
					}
					break;
				}
			}
		}
		Sleep(1);
	}

	DelUser(pUser);
	closesocket(sock);
	return 1;
}

int main()
{
	//ũ��Ƽ�� ���� �ʱ�ȭ
	//InitializeCriticalSection()<------> DeleteCriticalSection()
	InitializeCriticalSection(&g_Crit);

	if (BeginWinSock() == false)
		return -1;

	//���� ���� �Ϸ�, listenSock
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSock == INVALID_SOCKET)
		return 1;

	// ip + port -> bind -> � ü�� ����.
	// port�� ����ϰ� �ִ� ���μ���(���������� �����ϴ� �뵵�� �Ѵ�.)���� �˷��ش�.
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip�� �������� ��쿡(����, ������) �������� �ƹ��ŷ� �� �޴´ٴ� ��. INADDR_ANY�� ���°� �����ϴ�.
	//�� ������ ������ �ΰ� �̻��� IP �ּҸ� ���� ��� Ŭ���̾�Ʈ�� ��� �ּҷ� ������ �ϵ��� ó���Ҽ� �ֱ� �����̴�.

	//���ε�
	int ret = bind(listenSock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
		return 1;

	// listen -> ���. SOMAXCONN ���������� �ִ밪�� ���.
	ret = listen(listenSock, SOMAXCONN);	//->����
	if (ret == SOCKET_ERROR)
		return 1;

	//�����带 ������ ���� ���ŷ �Լ��� �ʿ� ����.
	//���� �����带 �ð��� �ɰ��� ������ ���⶧���� �� ������ �ʴ´�.
	//���� �Ʒ� �ڵ�� �ּ��ؾ� ���������� ���ư���.

	//FIONBIO =>File InputOutput NonBlocking InputOutput
	//listenSock�� ����ϴ� ��� API�Լ��� �� NonBlocking�Լ��� �ٲ���.
	//ClientSock�� NonBlocking �Ǵ°� �ƴϴ�.
	//���ŷ�Լ��� ����ϸ� ����ó���� ������Ѵ�. 
	//�������� ��ȯ���� �׳� ��ȯ���� ���� �ؾ� �Ѵ�.
	/*u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);*/

	/*map<int, TUser> g_userList;
	int g_iNumUser = 0;*/

	while (1)
	{
		//client socket�غ�
		//while(1)
		// clientSocket = accept(listenSocket)	//������ -> ���ӽõ�, ����õ� -> ����, �㰡

		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);

		//3��° �������ڰ� sizeof(clientInfo)�� �ƴ϶� �׺����� �ּҰ�(&addlen)�� ���Ѵ�.
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		AddUser(tUser);

		DWORD threadID;
		HANDLE hThread = CreateThread(0, 0, ClientThread, (LPVOID)&tUser.sock, 0, &threadID);

		if (tUser.sock != SOCKET_ERROR)
		{
			//Use inet_ntop() or InetNtop() headerfile <ws2tcpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n ���� ip = %s, port = %d",
				inet_ntop(AF_INET, &tUser.clientAddr.sin_addr, ip, INET_ADDRSTRLEN), ntohs(tUser.clientAddr.sin_port));
		}
		//���ŷ �Լ��ϰ���� ó���� ��Ƽ�������� ��� �ʿ� ����.
		////������ ��� ��ȯ�ǵ� SOCKET_ERROR�� �߰�, ��¥ Error�̿��� SOCKET_ERROR�̴�.
		//if (tUser.sock == SOCKET_ERROR)
		//{
		//	//WSA_E_WOULD_BLOCK = ������ ��� ��ȯ�� Error
		//	if (WSAGetLastError() != WSAEWOULDBLOCK)
		//	{
		//		exit(1);
		//	}
		//}
	}

	//������ ����� ��쿡, �����鿡�� �������� �޼����� ������ �����Ѵ�.
	char buffer2[256] = { 0, };
	for(int i = 0; i < g_allUser.size(); i++)
	{
		char msg[] = "���������մϴ�.!!!";
		int iRecvByte = sizeof(msg);

		send(g_allUser[i].sock, msg, iRecvByte, 0);
		closesocket(g_allUser[i].sock);
	}

	closesocket(listenSock);
	EndWinSock();

	DeleteCriticalSection(&g_Crit);
	std::cout << "Server Hello World\n";
	return 0;
}