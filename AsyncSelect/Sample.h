#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32")

using namespace std;

struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
	int addrlen;
	static SOCKET g_socket;

	bool operator()(TUser& user)
	{
		return (user.sock == g_socket);
	}

	TUser()
	{
		addrlen = sizeof(clientAddr);
	}
	TUser(SOCKET client, SOCKADDR_IN add)
	{
		sock = client;
		clientAddr = add;
		addrlen = sizeof(clientAddr);
	}
	TUser(const TUser& user)
	{
		sock = user.sock;
		clientAddr = user.clientAddr;
		addrlen = user.addrlen;
	}
	int iIndex;
};

SOCKET TUser::g_socket = NULL;
SOCKET g_listenSocket = NULL;

std::list<TUser> g_allUser;
typedef std::list<TUser>::iterator ITOR;

CRITICAL_SECTION g_Crit;
HANDLE g_hMutex;

void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

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

void AddUser(TUser& user)
{
	//EnterCriticalSection(&g_Crit);
	//EnterCriticalSection과 같다. 무한이 기다려준다.
	//무한대기하면 WAIT_TIMEOUT할 필요가 없다. 시간을 주어줄때 WAIT_TIMEOUT을 쓴다.
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
	if (dwRet != WAIT_TIMEOUT)
	{
		g_allUser.push_back(user);
	}
	//LeaveCriticalSection(&g_Crit);
	ReleaseMutex(g_hMutex);
}

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg);
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	while (sendbytes < iTotalsize)
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	}

	return iTotalsize;
}

int SendMsg(SOCKET sock, PACKET_HEADER ph, char* msg)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph = ph;
	memcpy(sendmsg.msg, msg, strlen(msg));

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	while (sendbytes < iTotalsize)
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	}

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	int iSend = 0;
	while (sendbytes < iTotalsize)
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	}

	return iTotalsize;
}

int Broadcastting(char* pMsg)
{
	//EnterCriticalSection(&g_Crit);
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
	
	//중요!!!!증감연산자가 없다. if문 안에서 증감한다.
	for (ITOR itor = g_allUser.begin(); itor != g_allUser.end();)
	{
		//SelectModel은 g_allUser 맨 앞에는 리슨소켓이 등록되어있다.
		//리슨소켓은 브로드캐스트하지 않는다.
		if (g_listenSocket == (*itor).sock)
		{
			itor++;
			continue;
		}

		TUser* pUser = (TUser*)&(*itor);
		if (pUser != NULL)
		{
			if (SendMsg(pUser->sock, pMsg, PACKET_CHAT_MSG) <= 0)
			{
				closesocket(pUser->sock);
				itor = g_allUser.erase(itor);
			}
			else
			{
				itor++;
			}
		}
	}
	ReleaseMutex(g_hMutex);
	return 1;
}

bool BeginWinSock()
{
	WSADATA wsa;
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0) return false;

	return true;
}
bool EndWinSock()
{
	int iRet = WSACleanup();
	if (iRet != 0)	return false;

	return true;
}

