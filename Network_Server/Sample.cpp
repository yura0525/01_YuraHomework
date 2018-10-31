#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <vector>
#include <map>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
};

void tMSG(TUser* pUser, const char* msg)
{
	printf("\n ip = %s, msg = %s", inet_ntoa(pUser->clientAddr.sin_addr), msg);
}

bool BeginWinSock()
{
	WSADATA wsa;
	//���� �ʱ�ȭ. 0�̸� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	return true;
}
bool EndWinSock()
{
	//��ü �Ҹ�
	if (WSACleanup() == 0)
		return false;

	return true;
}

int main()
{
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

	//FIONBIO =>File InputOutput NonBlocking InputOutput
	//listenSock�� ����ϴ� ��� API�Լ��� �� NonBlocking�Լ��� �ٲ���.
	//ClientSock�� NonBlocking �Ǵ°� �ƴϴ�.
	//���ŷ�Լ��� ����ϸ� ����ó���� ������Ѵ�. 
	//�������� ��ȯ���� �׳� ��ȯ���� ���� �ؾ� �Ѵ�.
	u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);

	map<int, TUser> g_userList;
	int g_iNumUser = 0;

	while (1)
	{
		//client socket�غ�
		//while(1)
		// clientSocket = accept(listenSocket)	//������ -> ���ӽõ�, ����õ� -> ����, �㰡
		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);

		//3��° �������ڰ� sizeof(clientInfo)�� �ƴ϶� �׺����� �ּҰ�(&addlen)�� ���Ѵ�.
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		//������ ��� ��ȯ�ǵ� SOCKET_ERROR�� �߰�, ��¥ Error�̿��� SOCKET_ERROR�̴�.
		if (tUser.sock == SOCKET_ERROR)
		{
			//WSA_E_WOULD_BLOCK = ������ ��� ��ȯ�� Error
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				exit(1);
			}
		}
		else
		{
			g_userList.insert(make_pair(g_iNumUser++, tUser));

			//Use inet_ntop() or InetNtop() headerfile <ws2tcpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n ip = %s, port = %d",
				inet_ntop(AF_INET, &tUser.clientAddr.sin_addr, ip, INET_ADDRSTRLEN), ntohs(tUser.clientAddr.sin_port));
		}
		char recvMsg[256] = { 0, };
		int iRecvByte = sizeof(recvMsg);

		map<int, TUser>::iterator iter;
		int iByte;
		for (iter = g_userList.begin(); iter != g_userList.end(); iter++)
		{
			TUser* pUser = &(iter->second);
			iByte = recv(pUser->sock, recvMsg, iRecvByte, 0);

			//�������� ������ ���.
			//map���� ����� ������ �ݴ´�.
			if (iByte == 0)
			{
				g_userList.erase(iter);
				closesocket(pUser->sock);
				tMSG(pUser, "���� �������ϴ�.\n");
				break;
			}

			if (iByte == SOCKET_ERROR)
			{
				//WSA_E_WOULD_BLOCK = ������ ��� ��ȯ�� Error
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					g_userList.erase(iter);
					closesocket(pUser->sock);
					break;
				}
			}
			else
			{
				tMSG(pUser, recvMsg);
				break;
			}
		}

		//��ε�ĳ��Ʈ
		for (iter = g_userList.begin(); iter != g_userList.end(); iter++)
		{
			TUser* pUser = &(iter->second);
			send(pUser->sock, recvMsg, iByte, 0);
		}
	}

	char buffer2[256] = { 0, };

	while (0 < g_userList.size())
	{
		char msg[] = "���������մϴ�.!!!";
		int iRecvByte = sizeof(msg);

		for (int iUser = 0; iUser < g_userList.size(); iUser++)
		{
			send(g_userList[iUser].sock, msg, iRecvByte, 0);
		}

		for (int iUser = 0; iUser < g_userList.size(); iUser++)
		{
			closesocket(g_userList[iUser].sock);
		}
	}

	closesocket(listenSock);
	EndWinSock();
	std::cout << "Server Hello World\n";
	return 0;
}