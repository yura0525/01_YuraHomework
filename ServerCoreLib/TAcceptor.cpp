#include "TAcceptor.h"
#include "TDebugString.h"
#include "TServer.h"
#include "TSynchronize.h"
#include "TServerIOCP.h"
#include <errno.h>

bool TAcceptor::Run()
{
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;
	
	while (m_bStarted)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			continue;
		}
		{
			TSynchronize sync(this);
			TCHAR buf[INET_ADDRSTRLEN] = { 0, };
			InetNtop(AF_INET, &clientaddr.sin_addr, buf, sizeof(buf));
			I_DebugStr.DisplayText("TAcceptor::Run() [IP:%s, PORT:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			I_Server.AddUser(client_sock);
		}
		Sleep(1);
	}
	m_bStarted = false;
	return true;
}
bool TAcceptor::Set(int iPort, const char* strAddress)
{
	int iRet;
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSock == INVALID_SOCKET)
		return false;

	int optval = 1;
	setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

	int option = TRUE;
	setsockopt(m_ListenSock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	if (strAddress == NULL)
	{
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		serveraddr.sin_addr.s_addr = inet_addr(strAddress);
	}
	serveraddr.sin_port = htons(iPort);
	iRet = bind(m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (iRet == SOCKET_ERROR)
		return false;

	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
		return false;

	CreateThread();
	return true;
}

int	 TAcceptor::GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int i;
	WSADATA data;

	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR)
		return 0;

	if (!::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);

		if (pHost != NULL)
		{
			for (i = 0; i < 255; i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE bAddr[4];
					char strIPAddr[256] = { 0, };
					bAddr[0] = (BYTE)pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE)pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE)pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE)pHost->h_addr_list[i][3];
					sprintf_s(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
					ipList.push_back(strIPAddr);
				}
				else
				{
					break;
				}
			}
		}
	}
	WSACleanup();
	return (int)ipList.size();
}
TAcceptor::TAcceptor()
{
}


TAcceptor::~TAcceptor()
{
}
