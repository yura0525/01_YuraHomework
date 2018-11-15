#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <vector>
#include <map>
#include "TPacket.h"
#include "Sample.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

TUser g_allUser[100];
int g_iNumClient = 0;
CRITICAL_SECTION g_Crit;

void AddUser(TUser& user)
{
	EnterCriticalSection(&g_Crit);
		g_allUser[g_iNumClient] = user;
		g_iNumClient++;
	LeaveCriticalSection(&g_Crit);
}

int Broadcastting(char* pMsg)
{
	EnterCriticalSection(&g_Crit);

	for (int iUser = 0; iUser < g_iNumClient; iUser++)
	{
		if (0 <= SendMsg(g_allUser[iUser].sock, pMsg, PACKET_CHAT_MSG))
		{
			continue;
		}
	}
	LeaveCriticalSection(&g_Crit);
	return 1;
}

void tMSG(TUser* pUser, const char* msg)
{
	printf("\n ip = %s, msg = %s", inet_ntoa(pUser->clientAddr.sin_addr), msg);
}

void DelUser(TUser* pUser)
{
	tMSG(pUser, " 퇴장하였습니다.\n");

	EnterCriticalSection(&g_Crit);
	for (int iUser = 0; iUser < g_iNumClient; iUser++)
	{
		if (g_allUser[iUser].sock == pUser->sock)
		{
			//지울때 뒤에서 하나씩 땡겨오고 g_iNumClient을 하나 줄인다.
			for (int iDel = iUser; iDel < g_iNumClient; iDel++)
			{
				g_allUser[iDel] = g_allUser[iDel + 1];
			}
			break;
		}
	}
	g_iNumClient--;

	closesocket(pUser->sock);
	LeaveCriticalSection(&g_Crit);
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

		//처음엔 PACKET_HEADER_SIZE사이즈만큼 받는다.
		iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
		if (iRet == 0)	break;
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		recvByte += iRet;

		//패킷을 PACKET_HEADER_SIZE만큼 받았을때.
		if (recvByte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(UPACKET));
			memcpy(&packet.ph, buffer, sizeof(char) * PACKET_HEADER_SIZE);

			//패킷을 packet.ph.len길이만큼 받는다. 패킷을 다받아야 while문 종료.
			//packet.ph.len에는 msg의 길이만 들어있다. PACKET_HEADER_SIZE 더해져있지 않다.
			int rByte = 0;
			do {
				
				int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char) * (packet.ph.len - rByte), 0);

				if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
				{
					bConnect = false;
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
						printf("패킷 완성 %s\n", packet.msg);
						Broadcastting(packet.msg);
					}
					break;
					case PACKET_CREATE_CHARACTER:
					{
						CHARACTER_INFO cInfo;
						memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
					}
					break;
				}
				printf("패킷 완성 %s\n", packet.msg);
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
	//크리티컬 섹션 초기화
	//InitializeCriticalSection()<------> DeleteCriticalSection()
	InitializeCriticalSection(&g_Crit);

	if (BeginWinSock() == false)
		return -1;

	//소켓 생성 완료, listenSock
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSock == INVALID_SOCKET)
		return 1;

	// ip + port -> bind -> 운영 체제 보고.
	// port를 사용하고 있는 프로세스(실행파일을 구분하는 용도로 한다.)에게 알려준다.
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip가 여러개인 경우에(랜선, 무선랜) 여러개중 아무거로 다 받는다는 뜻. INADDR_ANY을 쓰는게 적절하다.
	//그 이유는 서버가 두개 이상의 IP 주소를 가진 경우 클라이언트가 어느 주소로 접속을 하든지 처리할수 있기 때문이다.

	//바인딩
	int ret = bind(listenSock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
		return 1;

	// listen -> 듣다. SOMAXCONN 지원가능한 최대값을 사용.
	ret = listen(listenSock, SOMAXCONN);	//->개통
	if (ret == SOCKET_ERROR)
		return 1;

	//스레드를 여러개 쓰면 논블럭킹 함수가 필요 없다.
	//각자 스레드를 시간을 쪼개서 나눠서 쓰기때문에 블럭 당하지 않는다.
	//따라서 아래 코드는 주석해야 정상적으로 돌아간다.

	//FIONBIO =>File InputOutput NonBlocking InputOutput
	//listenSock이 사용하는 모든 API함수를 다 NonBlocking함수로 바꿔줌.
	//ClientSock이 NonBlocking 되는건 아니다.
	//논블럭킹함수를 사용하면 예외처리를 해줘야한다. 
	//정상적인 반환인지 그냥 반환인지 구분 해야 한다.
	/*u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);*/

	/*map<int, TUser> g_userList;
	int g_iNumUser = 0;*/

	while (1)
	{
		//client socket준비
		//while(1)
		// clientSocket = accept(listenSocket)	//누군가 -> 접속시도, 연결시도 -> 수용, 허가

		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);

		//3번째 전달인자가 sizeof(clientInfo)가 아니라 그변수의 주소값(&addlen)을 원한다.
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		AddUser(tUser);

		DWORD threadID;
		HANDLE hThread = CreateThread(0, 0, ClientThread, (LPVOID)&g_allUser[g_iNumClient-1], 0, &threadID);

		if (tUser.sock != SOCKET_ERROR)
		{
			//Use inet_ntop() or InetNtop() headerfile <ws2tcpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n 입장 ip = %s, port = %d",
				inet_ntop(AF_INET, &tUser.clientAddr.sin_addr, ip, INET_ADDRSTRLEN), ntohs(tUser.clientAddr.sin_port));
		}
		//논블럭킹 함수일경우의 처리라 멀티스레드일 경우 필요 없다.
		////할일이 없어서 반환되도 SOCKET_ERROR가 뜨고, 진짜 Error이여도 SOCKET_ERROR이다.
		//if (tUser.sock == SOCKET_ERROR)
		//{
		//	//WSA_E_WOULD_BLOCK = 할일이 없어서 반환된 Error
		//	if (WSAGetLastError() != WSAEWOULDBLOCK)
		//	{
		//		exit(1);
		//	}
		//}
	}

	//서버가 종료될 경우에, 유저들에게 게임종료 메세지를 보내고 종료한다.
	char buffer2[256] = { 0, };
	for(int i = 0; i < g_iNumClient; i++)
	{
		char msg[] = "게임종료합니다.!!!";
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