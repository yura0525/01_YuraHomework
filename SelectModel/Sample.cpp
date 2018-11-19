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
		printf("\n---->정상 퇴장\n");
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
				printf("\n패킷 완성 => %s\n", packet.msg);
				Broadcastting(packet.msg);
				break;
			}
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
				printf("\n패킷 완성 => %s\n", packet.msg);
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

	//ERROR_ALREADY_EXISTS :프로세스가 2개 중복 생성되는것을 막는다.
	//프로세스가 실행된지 모르고 2개 실행될때, 이미 실행되어있다고 막는다.
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

	// ip + port -> bind -> 운영체제보고.
	//port를 사용하고 있는 프로세스(실행파일을 구분하는용도로 한다.)에게 알려준다.
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);

	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip가 여러개인경우에 랜선, 무선랜 여러개중 아무거나 다 받는다는 뜻.
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	//listen -> 듣다.
	listen(listenSock, SOMAXCONN);	//->개통

	//리슨소켓도 유저에 등록한다.

	TUser listenUser(listenSock, sa);;
	AddUser(listenUser);

	FD_SET rSet;
	FD_SET wSet;
	FD_SET eSet;

	bool bEnding = false;

	while (bEnding == false)
	{
		//셋을 비운다(초기화)
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_ZERO(&eSet);

		for (ITOR itor = g_allUser.begin(); itor != g_allUser.end(); itor++)
		{
			TUser* pUser = (TUser*)&(*itor);
			if (pUser != NULL)
			{
				//리슨소켓등록
				//셋의 맨 앞에는 listenSock을 넣는다.
				FD_SET(pUser->sock, &rSet);
				FD_SET(pUser->sock, &wSet);
				FD_SET(pUser->sock, &eSet);
			}
		}

		//Select함수의 장점 : 
		//블록킹 소켓일 경우 -> 블록킹 함수 문제 해결.
		timeval time;
		time.tv_sec = 1;		//세컨드
		time.tv_usec = 0;		//마이크로세컨드
		
		int iRet = select(0, &rSet, &wSet, &eSet, &time);
		if (iRet == SOCKET_ERROR)
			break;

		if (iRet == 0)		//timeout
		{
			int time = 0;
		}
		else
		{
			//rSet에 listenSock이 있으면 0이 아닌값을 리턴한다.
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
				//if문 안에서 증감한다.
				for (ITOR itor = g_allUser.begin(); itor != g_allUser.end(); )
				{
					TUser* pUser = (TUser*)&(*itor);
					if (pUser != NULL)
					{
						SOCKET sock = pUser->sock;
						if (FD_ISSET(sock, &rSet))
						{
							//클라이언트 소켓일 경우 recv()
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
