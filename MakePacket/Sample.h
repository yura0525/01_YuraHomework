#define  _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once
#include <winsock2.h>
#include <iostream>
#include <map>
#include "TProtocol.h"
#include "TPacket.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

CRITICAL_SECTION	g_crit;			//유저 모드
HANDLE				g_hMutex;		//커널 모드
struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
	TUser() {}
	TUser(SOCKET client, SOCKADDR_IN addr)
	{
		sock = client;
		clientAddr = addr;
	}
	TUser(const TUser& user)
	{
		sock = user.sock;
		clientAddr = user.clientAddr;
	}
};

map<SOCKET, TUser> g_allClientSocket;
typedef map<SOCKET, TUser>::iterator ITOR;

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
	if (iRet == 0)
	{
		printf("\n---->정상퇴장");
		return 0;
	}
	if (iRet <= SOCKET_ERROR)
	{
		printf("\n---->비정상 퇴장");
		T_ERROR();
		return -1;
	}
	return iRet;
}

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

	char* pMsg = (char*)&sendMsg;
	do{
		iSend = send(sock, (char*)&pMsg[sendBytes], iTotalSize - sendBytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);
	
	return sendBytes;
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
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);

	return sendBytes;
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
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendBytes += iSend;
	} while (sendBytes < iTotalSize);

	return sendBytes;
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

int PacketSend(SOCKET sock, char* msg, WORD type)
{
	TPacket packet(type);
	packet << msg;

	int sendbytes = 0;
	int iTotalsize = packet.m_uPacket.ph.len + PACKET_HEADER_SIZE;

	char* pMsg = (char*)&packet.m_uPacket;
	int iSend = 0;
	do {
		iSend = send(sock, &pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return sendbytes;
}

int Broadcastting(UPACKET* pPacket)
{
	int iUserCounter = 0;
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);			//대기 함수
	if (dwRet != WAIT_TIMEOUT)
	{
		for (ITOR itor = g_allClientSocket.begin(); itor != g_allClientSocket.end();)
		{
			TUser* pUser = (TUser*)&itor->second;
			if (pUser != NULL)
			{
				int iSendByte = PacketSend(pUser->sock, (char*)pPacket, PACKET_CHAT_MSG);
				if (CheckReturn(iSendByte) <= 0)
				{
					printf("\n퇴장(Send) : IP:%s, PORT:%d\n",
						inet_ntoa(pUser->clientAddr.sin_addr), ntohs(pUser->clientAddr.sin_port));
					g_allClientSocket.erase(itor++);
				}
				else
				{
					iUserCounter++;
					++itor;
				}
			}
		}
	}
	ReleaseMutex(g_hMutex);
	return -1;
}

void DeleteUser(SOCKET sock)
{
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);			//대기 함수
	ITOR itor = g_allClientSocket.find(sock);

	if (itor != g_allClientSocket.end())
	{
		TUser* pUser = (TUser*)&itor->second;
		if (pUser != NULL)
		{
			printf("\n퇴장(Send) : IP:%s, PORT:%d\n",
				inet_ntoa(pUser->clientAddr.sin_addr), ntohs(pUser->clientAddr.sin_port));
		}
		g_allClientSocket.erase(itor);
	}

	closesocket(sock);
	ReleaseMutex(g_hMutex);
}

void GreetMessage(TUser& user)
{
	printf("\n클라이언트 접속 : IP:%s, PORT:%d\n",
		inet_ntoa(user.clientAddr.sin_addr), ntohs(user.clientAddr.sin_port));

	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[서버]환영합니다.");
	PacketSend(user.sock, message, PACKET_CHAT_MSG);

	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);		// 대기함수
	g_allClientSocket.insert(make_pair(user.sock, user));
	ReleaseMutex(g_hMutex);
}