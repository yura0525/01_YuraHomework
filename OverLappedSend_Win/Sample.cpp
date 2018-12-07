#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")
using namespace std;

#define BUF_SIZE 100


void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}

void main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAddr;

	WSABUF dataBuf;
	char msg[] = "Network is Computer!";
	int sendBytes = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		T_ERROR();
		return;
	}

	hSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr("192.168.0.27");
	sendAddr.sin_port = htons(10000);
}