#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")
using namespace std;

#define BUF_SIZE 1024

void ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('n', stderr);
	exit(1);
}

void main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAddr;
	char message[BUF_SIZE];
	int strLen, readLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!!!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	ZeroMemory(&sendAddr, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr("192.168.0.27");
	sendAddr.sin_port = htons(10000);

	if (connect(hSocket, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!!!");
	}
	else
	{
		puts("Connected....");
	}

	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = strlen(message);
		send(hSocket, message, strLen, 0);
		readLen = 0;
		while (1)
		{
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen)
				break;
		}
		message[strLen] = NULL;
		printf("Message from server : %s", message);
	}

	closesocket(hSocket);
	WSACleanup();
}
