#include <winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

vector<SOCKET> g_userList;
int main()
{
	WSADATA wd;
	//윈속 초기화. 0이면 성공.
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//소켓 생성 완료
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		return 1;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	//서버의 경우 특정 IP주소를 대입하기보다는 INADDR_ANY을 사용하는것이 바람직하다.
	//그 이유는 서버가 두개 이상의 IP 주소를 가진경우 
	//클라이언트가 어느 주소로 접속을 하든지 처리할수 있기 때문이다.

	//바인딩
	int ret = bind(sock, (sockaddr*), sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	//리슨. SOMAXCONN 지원가능한 최대값을 사용.
	ret = listen(sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientInfo;
	int addlen = sizeof(clientInfo);

	while (1)
	{
		//3번째 전달인자가 sizeof(clientInfo)가 아니라 그변수위 주소값(&addlen)을 원한다.
		clientSock = accept(sock, (sockaddr*)&clientInfo, &addlen);

		if (clientSock == INVALID_SOCKET)
			break;

		printf("\n %s, %d", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
		g_userList.push_back(clientSock);

		char buffer2[256] = { 0, };

		while (1)
		{
			ZeroMemory(buffer2, sizeof(char) * 256);
			ret = recv(clientSock, buffer2, sizeof(buffer2), 0);
			
			if (ret == 0)	//정상종료시 recv는 0을 리턴한다.
			{
				printf("\n정상 종료 %s, %d 나갔습니다.", 
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
		}
	}
}