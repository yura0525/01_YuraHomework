#include <winsock2.h>
#include <iostream>
#pragma comment (lib, "ws2_32.lib")


int main()
{
	WSADATA wd;
	//윈속 초기화. 0이면 성공
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//소켓 생성 완료
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);	//TCP

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	addr.sin_port = htons(10000);
	
	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	char buffer[256] = { 0, };
	int iLen = 0;

	while (1)
	{
		ZeroMemory(buffer, sizeof(char) * 256);
		fgets(buffer, 256, stdin);

		//enter지움.
		buffer[strlen(buffer) - 1] = '\0';

		send(sock, buffer, strlen(buffer), 0);
	}
}


