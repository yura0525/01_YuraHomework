#pragma once
#include <winsock2.h>
#include <iostream>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32.lib")

struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
};

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendMsg;
	ZeroMemory(&sendMsg, sizeof(sendMsg));

	sendMsg.ph.len = strlen(msg);
	sendMsg.ph.type = type;
	memcpy(sendMsg.msg, msg, strlen(msg));

	int sendBytes = 0;
	int iTotalSize = strlen(msg) + PACKET_HEADER_SIZE;
	int iSend = 0;

	printf("Broadcastting sendMsg.msg[%s], iTotalSize[%d]\n", sendMsg.msg, iTotalSize);
	//덜보냈으면 잘라서 보낸다.
	char* pMsg = (char*)&sendMsg;
	do {
		iSend = send(sock, (char*)&pMsg[sendBytes], iTotalSize - sendBytes, 0);
		if (iSend == SOCKET_ERROR)
			return iSend;

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);
	
	printf("Broadcastting pMsg[%s], sendBytes[%d]\n", pMsg, sendBytes);
	return iTotalSize;
}

int SendMsg(SOCKET sock, PACKET_HEADER ph, char* msg)
{
	UPACKET sendMsg;
	ZeroMemory(&sendMsg, sizeof(sendMsg));
	sendMsg.ph = ph;

	memcpy(sendMsg.msg, msg, strlen(msg));

	int sendBytes = 0;
	int iTotalSize = strlen(msg) + PACKET_HEADER_SIZE;
	int iSend = 0;

	//덜보냈으면 잘라서 보낸다.
	char* pMsg = (char*)&sendMsg;
	do {
		iSend = send(sock, (char*)&pMsg[sendBytes], iTotalSize - sendBytes, 0);
		if (iSend == SOCKET_ERROR)
			return iSend;

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);

	return iTotalSize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendBytes = 0;
	int iTotalSize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	int iSend = 0;

	//덜보냈으면 잘라서 보낸다.
	char* pMsg = (char*)uPacket;
	do {
		iSend = send(sock, (char*)&pMsg[sendBytes], iTotalSize - sendBytes, 0);
		if (iSend == SOCKET_ERROR)
			return iSend;

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);

	return iTotalSize;
}

void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}

int CheckReturn(int iRet)
{
	//정상
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
	if (iRet != 0) return false;

	return true;
}