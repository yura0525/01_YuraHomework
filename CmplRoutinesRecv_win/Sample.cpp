#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")
using namespace std;

#define BUF_SIZE 1024

void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

WSABUF dataBuf;
char buf[BUF_SIZE];
DWORD recvBytes = 0;

int main()
{
	WSADATA wsaData;
	SOCKET hListenSock, hRecvSock;
	SOCKADDR_IN listenAddr, recvAddr;

	WSAOVERLAPPED overlapped;
	WSAEVENT evObj;

	DWORD idx, flags = 0;
	int recvAddrSize;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&listenAddr, sizeof(listenAddr));

	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(10000);

	if (bind(hListenSock, (SOCKADDR*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	if (listen(hListenSock, SOMAXCONN) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	recvAddrSize = sizeof(recvAddr);
	hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAddr, &recvAddrSize);
	if (hRecvSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");

	ZeroMemory(&overlapped, sizeof(overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;
	evObj = WSACreateEvent();	//Dummy eventObject;

	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
			puts("Background data receive");
	}

	idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
	if (idx == WAIT_IO_COMPLETION)
		puts("Overlapped I/O Completed");
	else
		ErrorHandling("WSARecv() error!");

	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hListenSock);
	WSACleanup();
}

void CALLBACK CompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError != 0)
	{
		ErrorHandling("CompRoutine error!");
	}
	else
	{
		szRecvBytes = szRecvBytes;
		printf("Received message: %s\n", buf);
	}
}

void ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('n', stderr);
	exit(1);
}