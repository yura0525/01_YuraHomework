#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")
using namespace std;

#define BUF_SIZE 1024

void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);

void ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('n', stderr);
	exit(1);
}

typedef struct
{
	SOCKET hClientSock;
	char buf[BUF_SIZE];
	WSABUF wsaBuf;
}PER_IO_DATA, *LPPER_IO_DATA;

void main()
{
	WSADATA wsaData;
	SOCKET hListenSock, hRecvSock;
	SOCKADDR_IN listenAddr, recvAddr;
	LPWSAOVERLAPPED lpOvLP;
	DWORD recvBytes, flagInfo = 0;
	LPPER_IO_DATA hbInfo;

	u_long on = TRUE;
	int recvAddrSize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() Error!!!");
	}

	hListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ioctlsocket(hListenSock, FIONBIO, &on);			//non blocking socket

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
	while (1)
	{
		SleepEx(100, TRUE);							//for alertable Wait State
		hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAddr, &recvAddrSize);
		if (hRecvSock == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else
				ErrorHandling("accept() error");
		}
		puts("Clientconnected...");
		lpOvLP = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
		memset(lpOvLP, 0, sizeof(WSAOVERLAPPED));

		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		hbInfo->hClientSock = hRecvSock;
		(hbInfo->wsaBuf).buf = hbInfo->buf;
		(hbInfo->wsaBuf).len = BUF_SIZE;

		lpOvLP->hEvent = (HANDLE)hbInfo;
		WSARecv(hRecvSock, &(hbInfo->wsaBuf), 1, &recvBytes, &flagInfo, lpOvLP, ReadCompRoutine);
	}
	closesocket(hRecvSock);
	closesocket(hListenSock);
	WSACleanup();
}

void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCKET hSock = hbInfo->hClientSock;
	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
	DWORD sendBytes;

	if (szRecvBytes == 0)
	{
		closesocket(hSock);
		free(lpOverlapped->hEvent);
		free(lpOverlapped);
		puts("Client disconnected...");
	}
	else
	{
				//echo!!!
		bufInfo->len = szRecvBytes;
		WSASend(hSock, bufInfo, 1, &sendBytes, 0, lpOverlapped, WriteCompRoutine);
	}
}

void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szSendBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCKET hSock = hbInfo->hClientSock;
	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
	DWORD recvBytes;
	DWORD flagInfo = 0;

	WSARecv(hSock, bufInfo, 1, &recvBytes, &flagInfo, lpOverlapped, ReadCompRoutine);
}
