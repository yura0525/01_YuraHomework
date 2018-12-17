#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>
#include<windows.h>

#pragma comment(lib, "ws2_32")
using namespace std;

#define BUF_SIZE 1024
#define READ 3
#define WRITE 5

typedef struct //socket info
{
	SOCKET hClientSock;
	SOCKADDR_IN clientAddr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct //buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;				//READ or WRITE
}PER_IO_DATA, *LPPER_IO_DATA;

void ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('n', stderr);
	exit(1);
}

DWORD WINAPI EchoThreadMain(LPVOID pComPort);

void main()
{
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAddr;
	DWORD recvBytes, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	for (int i = 0; i < sysInfo.dwNumberOfProcessors; i++)
	{
		CreateThread(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);
	}

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(10000);

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, SOMAXCONN);

	while (1)
	{
		SOCKET hClientSock;
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);

		hClientSock = accept(hServSock, (SOCKADDR*)&clientAddr, &addrLen);
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));

		handleInfo->hClientSock = hClientSock;
		memcpy(&(handleInfo->clientAddr), &clientAddr, addrLen);

		CreateIoCompletionPort((HANDLE)hClientSock, hComPort, (ULONG_PTR)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		ZeroMemory(&(ioInfo->overlapped), sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClientSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}
	return;
}

DWORD WINAPI EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock = handleInfo->hClientSock;

		if (ioInfo->rwMode == READ)
		{
			puts("message received");
			if (bytesTrans == 0)
			{
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}

			ZeroMemory(&(ioInfo->overlapped), sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;

			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			ZeroMemory(&(ioInfo->overlapped), sizeof(OVERLAPPED));

			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;

			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else
		{
			puts("message sent!");
			free(ioInfo);
		}
	}
	return 0;
}
