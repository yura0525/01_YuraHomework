#include "sample.h"
#define WM_KGCA (WM_USER+0)

int RecvData(TUser* pUser)
{
	char buffer[256] = { 0, };
	int recvByte = 0;
	SOCKET sock = pUser->sock;
	int bConnect = true;

	int iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
	if (CheckReturn(iRet) <= 0)
	{
		printf("\n---->���� ����\n");
		return -1;
	}

	recvByte += iRet;

	if (recvByte == PACKET_HEADER_SIZE)
	{
		UPACKET packet;
		ZeroMemory(&packet, sizeof(UPACKET));
		memcpy(&packet.ph, buffer, sizeof(char)*PACKET_HEADER_SIZE);

		int rByte = 0;
		do
		{
			int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char) * (packet.ph.len - rByte), 0);

			if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
			{
				bConnect = false;
				T_ERROR();
				break;
			}
			rByte += iRecvByte;
		} while (rByte < packet.ph.len);

		recvByte = 0;
		if (bConnect)
		{
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				printf("\n��Ŷ �ϼ� => %s\n", packet.msg);
				Broadcastting(packet.msg);
				break;
			}
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
				printf("\n��Ŷ �ϼ� => %s\n", packet.msg);
				break;
			}
			}
		}
	}

	if (bConnect == false)
		return -1;
	return 1;
}

int main()
{
	//InitializeCriticalSection(&g_Crit);
	g_hMutex = CreateMutex(NULL, FALSE, L"KGCA");

	//ERROR_ALREADY_EXISTS :���μ����� 2�� �ߺ� �����Ǵ°��� ���´�.
	//���μ����� ������� �𸣰� 2�� ����ɶ�, �̹� ����Ǿ��ִٰ� ���´�.
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(g_hMutex);
		return 1;
	}

	if (BeginWinSock() == false)
	{
		return -1;
	}

	//listen socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, 0);

	// ip + port -> bind -> �ü������.
	//port�� ����ϰ� �ִ� ���μ���(���������� �����ϴ¿뵵�� �Ѵ�.)���� �˷��ش�.
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);

	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip�� �������ΰ�쿡 ����, ������ �������� �ƹ��ų� �� �޴´ٴ� ��.
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	//listen -> ���.
	listen(listenSock, SOMAXCONN);	//->����

	//�������ϵ� ������ ����Ѵ�.

	TUser listenUser(listenSock, sa);;
	AddUser(listenUser);

	FD_SET rSet;
	FD_SET wSet;
	FD_SET eSet;

	bool bEnding = false;

	while (bEnding == false)
	{
		//���� ����(�ʱ�ȭ)
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_ZERO(&eSet);

		for (ITOR itor = g_allUser.begin(); itor != g_allUser.end(); itor++)
		{
			TUser* pUser = (TUser*)&(*itor);
			if (pUser != NULL)
			{
				//�������ϵ��
				//���� �� �տ��� listenSock�� �ִ´�.
				FD_SET(pUser->sock, &rSet);
				FD_SET(pUser->sock, &wSet);
				FD_SET(pUser->sock, &eSet);
			}
		}

		//Select�Լ��� ���� : 
		//���ŷ ������ ��� -> ���ŷ �Լ� ���� �ذ�.
		timeval time;
		time.tv_sec = 1;		//������
		time.tv_usec = 0;		//����ũ�μ�����
		
		int iRet = select(0, &rSet, &wSet, &eSet, &time);
		if (iRet == SOCKET_ERROR)
			break;

		if (iRet == 0)		//timeout
		{
			int time = 0;
		}
		else
		{
			//rSet�� listenSock�� ������ 0�� �ƴѰ��� �����Ѵ�.
			if (FD_ISSET(listenSock, &rSet))
			{
				TUser addUser;
				addUser.sock = accept(listenSock, (SOCKADDR*)&addUser.clientAddr, &addUser.addrlen);
				if (addUser.sock == INVALID_SOCKET)
				{
					bEnding = true;
					break;
				}
				AddUser(addUser);
			}
			else
			{
				//if�� �ȿ��� �����Ѵ�.
				for (ITOR itor = g_allUser.begin(); itor != g_allUser.end(); )
				{
					TUser* pUser = (TUser*)&(*itor);
					if (pUser != NULL)
					{
						SOCKET sock = pUser->sock;
						if (FD_ISSET(sock, &rSet))
						{
							//Ŭ���̾�Ʈ ������ ��� recv()
							if (RecvData(pUser) <= 0)
							{
								closesocket(pUser->sock);
								itor = g_allUser.erase(itor);
								continue;
							}
						}
						if (FD_ISSET(sock, &wSet))
						{
							int kkk = 0;
						}
						if (FD_ISSET(sock, &eSet))
						{
							closesocket(pUser->sock);
							itor = g_allUser.erase(itor);
							continue;
						}
						itor++;
					}
				}
			}	
		}
	}
	closesocket(listenSock);
	EndWinSock();
	CloseHandle(g_hMutex);
}
