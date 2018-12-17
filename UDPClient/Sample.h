#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include"TProtocol.h"

#pragma comment(lib, "ws2_32.lib")

struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
};

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg);
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));

	SOCKADDR_IN recvAddr;
	ZeroMemory(&recvAddr, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	recvAddr.sin_addr.s_addr = inet_addr("192.168.0.101");
	recvAddr.sin_port = htons(9000);

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	int iSend = 0;
	char* pMsg = (char*)&sendmsg;
	do {
		iSend = sendto(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0, (sockaddr*)&recvAddr, sizeof(recvAddr));
		if (iSend == SOCKET_ERROR) { return iSend; }
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, PACKET_HEADER ph, char* msg)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph = ph;
	memcpy(sendmsg.msg, msg, strlen(msg));

	SOCKADDR_IN recvAddr;
	ZeroMemory(&recvAddr, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	recvAddr.sin_addr.s_addr = inet_addr("192.168.0.101");
	recvAddr.sin_port = htons(9000);

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do {
		iSend = sendto(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0, (sockaddr*)&recvAddr, sizeof(recvAddr));
		if (iSend == SOCKET_ERROR) { return iSend; }

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	
	SOCKADDR_IN recvAddr;
	ZeroMemory(&recvAddr, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	recvAddr.sin_addr.s_addr = inet_addr("192.168.0.101");
	recvAddr.sin_port = htons(9000);

	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	int iSend = 0;
	do {
		iSend = sendto(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0, (sockaddr*)&recvAddr, sizeof(recvAddr));
		if (iSend == SOCKET_ERROR) { return iSend; }

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}
int CheckReturn(int iRet)
{
	//Á¤»ó
	if (iRet == 0)
	{
		return 0;
	}
	if (iRet <= SOCKET_ERROR)
	{
		T_ERROR();
		return -1;
	}

	return iRet;
}

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

bool BeginWinSock()
{
	WSADATA wsa;

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0)	return false;

	return true;
}
bool EndWinSock()
{
	int iRet = WSACleanup();
	if (iRet != 0)	return false;

	return true;
}