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
	//윈속 초기화. 0이면 성공.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	return true;
}
bool EndWinSock()
{
	//객체 소멸
	if (WSACleanup() == 0)
		return false;

	return true;
}

int main()
{
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

	//FIONBIO =>File InputOutput NonBlocking InputOutput
	//listenSock이 사용하는 모든 API함수를 다 NonBlocking함수로 바꿔줌.
	//ClientSock이 NonBlocking 되는건 아니다.
	//논블럭킹함수를 사용하면 예외처리를 해줘야한다. 
	//정상적인 반환인지 그냥 반환인지 구분 해야 한다.
	u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);

	map<int, TUser> g_userList;
	int g_iNumUser = 0;

	while (1)
	{
		//client socket준비
		//while(1)
		// clientSocket = accept(listenSocket)	//누군가 -> 접속시도, 연결시도 -> 수용, 허가
		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);

		//3번째 전달인자가 sizeof(clientInfo)가 아니라 그변수의 주소값(&addlen)을 원한다.
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		//할일이 없어서 반환되도 SOCKET_ERROR가 뜨고, 진짜 Error이여도 SOCKET_ERROR이다.
		if (tUser.sock == SOCKET_ERROR)
		{
			//WSA_E_WOULD_BLOCK = 할일이 없어서 반환된 Error
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

			//누군가가 나갔을 경우.
			//map에서 지우고 소켓을 닫는다.
			if (iByte == 0)
			{
				g_userList.erase(iter);
				closesocket(pUser->sock);
				tMSG(pUser, "님이 나갔습니다.\n");
				break;
			}

			if (iByte == SOCKET_ERROR)
			{
				//WSA_E_WOULD_BLOCK = 할일이 없어서 반환된 Error
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

		//브로드캐스트
		for (iter = g_userList.begin(); iter != g_userList.end(); iter++)
		{
			TUser* pUser = &(iter->second);
			send(pUser->sock, recvMsg, iByte, 0);
		}
	}

	char buffer2[256] = { 0, };

	while (0 < g_userList.size())
	{
		char msg[] = "게임종료합니다.!!!";
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