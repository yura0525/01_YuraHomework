#include "TAcceptor_2.h"
#include "TDebugString_2.h"
#include "TSynchronize_1.h"
#include "TServer_1.h"
#include "TServerIOCP_1.h"
#include <errno.h>
bool TAcceptor_2::Run()
{
	int iRet;
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
		TCHAR buf[INET_ADDRSTRLEN];/*, buf6[INET6_ADDRSTRLEN]*/
		InetNtop(AF_INET, &clientaddr.sin_addr, buf, sizeof(buf));
		I_DebugStr.DisplayText(const_cast<char*>("\n[IP:%s,PORT:%d]\n"), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		{
			TSynchronize_1 sync(this);
			TUser*	pUser = NULL;
			SAFE_NEW(pUser, TUser);
			pUser->m_Socket = client_sock;

			I_ServerIOCP.AddHandleToIOCP((HANDLE)client_sock, (ULONG_PTR)pUser);

			ZeroMemory(&(pUser->m_ov), sizeof(pUser->m_ov));
			pUser->m_ov.m_iFlags = OVERLAPPED2::MODE_SEND;
			pUser->m_iEvent = pUser->m_Socket;

			tGUID uuid;
			RPC_STATUS ret_val = ::UuidCreate(&uuid);
			pUser->m_Guid = uuid;
			I_Server.m_UserList.insert(make_pair(uuid, pUser));

			// 환영메세지
			UPACKET sendmsg;
			ZeroMemory(&sendmsg, sizeof(sendmsg));
			sprintf(sendmsg.msg, "\n[서버]환영합니다. 대화명을 입력해 주세요\n");
			sendmsg.ph.type = PACKET_CHAT_NAME_REQ;
			sendmsg.ph.len = sizeof(PACKET_HEADER) + strlen(sendmsg.msg);
			ZeroMemory(&pUser->m_strBuffer, sizeof(char) * 2048);
			pUser->m_wsaBuffer.buf = (char*)&sendmsg;
			pUser->m_wsaBuffer.len = sendmsg.ph.len;

			DWORD dwSendByte = sendmsg.ph.len;
			iRet = WSASend(client_sock, &(pUser->m_wsaBuffer), 1, &dwSendByte, 0, (LPOVERLAPPED)&(pUser->m_ov), NULL);

			if (iRet == SOCKET_ERROR)
			{
				I_DebugStr.T_ERROR();
			}
		}
		Sleep(1);
	}
	m_bStarted = false;
	// 스레드 시작 함수가 리턴되면 스레드는 소멸됨.
	return 0;
}
bool TAcceptor_2::Set(int iPort, const char* strAddress)
{
	int iRet;
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ListenSock == INVALID_SOCKET)
	{
		return -1;
	}

	int optval = 1;
	setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

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
	{
		return -1;
	}

	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return -1;
	}

	CreateThread();
	return true;
}

int	 TAcceptor_2::GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int  i;
	WSADATA WsaData;
	if (WSAStartup(MAKEWORD(2,2), &WsaData) == SOCKET_ERROR)
	{
		return 0;
	}
	if (!::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);

		if (pHost != NULL)
		{
			for (i = 0; i<255; i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE    bAddr[4];
					char    strIPAddr[256] = { 0, };
					bAddr[0] = (BYTE)pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE)pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE)pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE)pHost->h_addr_list[i][3];
					sprintf(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
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
	return ipList.size();
}
TAcceptor_2::TAcceptor_2()
{
}


TAcceptor_2::~TAcceptor_2()
{
}
