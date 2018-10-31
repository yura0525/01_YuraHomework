#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wd;

	//�����ʱ�ȭ. 0�̸� ����
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//���� ���� �Ϸ�.
	//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//UDP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);				//TCP

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

	char buffer2[256] = { 0, };
	int iLen = 0;

	while (1)
	{
		ZeroMemory(buffer2, sizeof(char) * 256);
		//Ű������ �Է��� �޴´�.
		fgets(buffer2, 256, stdin);

		//enter����
		buffer2[strlen(buffer2) - 1] = '\0';

		//�����͸� �����Ѵ�.
		send(sock, buffer2, strlen(buffer2), 0);
		printf("[%s] : %zd ����Ʈ�� �����Ͽ����ϴ�.\n", buffer2, strlen(buffer2));

		ZeroMemory(buffer2, sizeof(char) * 256);
		recv(sock, buffer2, sizeof(buffer2), 0);
		printf("[%s] : %zd ����Ʈ�� �޾ҽ��ϴ�.\n", buffer2, strlen(buffer2));
	}

	closesocket(sock);
	WSACleanup();
	std::cout << "Client Hello World\n";
}