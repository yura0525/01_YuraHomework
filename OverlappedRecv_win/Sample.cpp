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
	SOCKET hListenSock, hRecvSock;
	SOCKADDR_IN listenAddr, recvAddr;
	int recvAddrSize;

	WSABUF dataBuf;
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	char buf[BUF_SIZE];
	DWORD recvBytes = 0, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() Error!!!");
	}

	hListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(10000);

	if (bind(hListenSock, (SOCKADDR*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error!!!");
	}

	if (listen(hListenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error!!!");
	}

	recvAddrSize = sizeof(recvAddr);
	hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAddr, &recvAddrSize);

	evObj = WSACreateEvent();
	ZeroMemory(&overlapped, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data receive");
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hRecvSock, &overlapped, &recvBytes, FALSE, NULL);
		}
		else
		{
			ErrorHandling("WSARecv() error");
		}
	}

	printf("Received message: %s\n", buf);
	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hListenSock);
	WSACleanup();
	return;
}