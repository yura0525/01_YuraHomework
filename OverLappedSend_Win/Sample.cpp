#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32")
using namespace std;


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

	WSABUF dataBuf;
	char msg[] = "Network is Computer!";
	DWORD sendBytes = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!!!");
	}

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&sendAddr, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr("192.168.0.27");
	sendAddr.sin_port = htons(10000);

	if (connect(hSocket, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!!!");
	}

	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.len = strlen(msg) + 1;
	dataBuf.buf = msg;

	if ( WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			printf("Background data send");
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);
		}
		else
		{
			ErrorHandling("WSASend error!!!");
			return;
		}
	}

	printf("Send Data Size : %d\n", sendBytes);
	getchar();
	WSACloseEvent(evObj);
	closesocket(hSocket);
	WSACleanup();
	return;
}

