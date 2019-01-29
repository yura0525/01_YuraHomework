#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include "TAsyncSelect.h"

#pragma comment(lib, "ws2_32")
using namespace std;

// ����� ���� �޼���( ��Ʈ��ũ �̺�Ʈ�� �߻��ϸ� ) 
#define NETWORK_MSG		WM_USER+80
// �񵿱⼿��Ʈ(AsyncSelect())
int TAsyncSelect::SendMsg()
{
	if (m_bSendEnable == false) return 0;
	if (m_SendList.size() == 0) return 0;
	std::list<UPACKET>::iterator itor = m_SendList.begin();
	UPACKET data = *itor;
	{
		DWORD dwSendSize;
		DWORD dwflag = 0;
		WSABUF wsabuf;
		wsabuf.len = data.ph.len;
		wsabuf.buf = (char*)&data;

		int iRet = WSASend(m_Sock, &wsabuf, 1, &dwSendSize, dwflag, NULL, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAEWOULDBLOCK == WSAGetLastError())
			{
				m_bSendEnable = FALSE;
				return 0;
			}
			return -1;
		}
		else
		{
			// �߻� ����!
			if (wsabuf.len != dwSendSize)
			{
				MessageBox(g_hWnd, L"WSASend", L"ERROR", MB_OK);
				return -1;
			}
			m_SendList.pop_front();
		}
	}
	return 0;
}
int TAsyncSelect::SendMsg(char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // ��� 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// ��� 2
	//sprintf_s(sendmsg.msg,"%s", "[����]ȯ���մϴ�.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = 0;

	if (m_bSendEnable == true)
	{
		iRet = send(m_Sock, (char*)&sendmsg, sendmsg.ph.len, 0);
	}
	else
	{
		Push(sendmsg);
	}

	return iRet;
}
int TAsyncSelect::SendMsg(SOCKET sock, char* msg, WORD type)
{
	if (m_bSendEnable == false) return 0;
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // ��� 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// ��� 2
	//sprintf_s(sendmsg.msg,"%s", "[����]ȯ���մϴ�.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;

	DWORD dwSendSize;
	DWORD dwflag = 0;
	WSABUF wsabuf;
	wsabuf.len = sendmsg.ph.len;
	wsabuf.buf = (char*)&sendmsg;

	int iRet = WSASend(m_Sock, &wsabuf, 1, &dwSendSize, dwflag, NULL, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			m_bSendEnable = FALSE;
			m_SendList.push_back(sendmsg);
			return 0;
		}
		return -1;
	}
	else
	{
		// �߻� ����!
		if (wsabuf.len != dwSendSize)
		{
			MessageBox(g_hWnd, L"WSASend", L"ERROR", MB_OK);
			return -1;
		}
	}
	return iRet;
}

int TAsyncSelect::RecvMsg()
{
	int iLen = 0;
	DWORD dwRecvSize;
	DWORD dwFlag = 0;
	WSABUF wsabuf;
	wsabuf.len = 2048;
	wsabuf.buf = m_szData;
	DWORD dwResult = WSARecv(m_Sock, &wsabuf, 1, &dwRecvSize, &dwFlag, NULL, NULL);
	//dwRecvSize==0 ó���� ���ʿ��ϴ�.
	if (dwResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return WSAEWOULDBLOCK;
	}
	else
	{
		m_StreamPacket.Put(m_szData, dwRecvSize);
	}
	return 0;
}

void TAsyncSelect::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case NETWORK_MSG:
		// ������ ����Ǿ ���´�.
		if (WSAGETSELECTERROR(msg.lParam) != 0)
		{
			MessageBox(g_hWnd, L"���� ����", L"����", MB_OK);
			PostQuitMessage(0);
			return;
		}
		WORD dwSelect = WSAGETSELECTEVENT(msg.lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:
		{
			m_bConnect = true;
			PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);
		}
		break;
		case FD_CLOSE:
		{
			m_bConnect = false;
		}
		break;
		case FD_READ:
		{
			int iRet = RecvMsg();
			// ������ ������ �ְų� ���� �����
			if (iRet < 0)
			{
				closesocket(m_Sock);
				break;
			}
			if (iRet != WSAEWOULDBLOCK)
			{
				PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);
			}
		}
		// �߿� break�� �����Ѵ�.
		case FD_WRITE:
		{
			m_bSendEnable = true;
			int iRet = SendMsg();
			if (iRet < 0)
				closesocket(m_Sock);
		}
		break;
		}
	}
}
bool TAsyncSelect::Connect(char* ip, int iPort)
{
	//(2)�񵿱� ����Ʈ�� ������ �޼��� ����
	int iResult = WSAAsyncSelect(m_Sock, g_hWnd, NETWORK_MSG, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	if (iResult == SOCKET_ERROR)
	{
		return false;
	}

	SOCKADDR_IN server;
	ZeroMemory(&server, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(ip);


	DWORD dwRet = WSAConnect(m_Sock, (sockaddr*)&server, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL);
	if (SOCKET_ERROR == dwRet)
	{
		// �񵿱� ���� ��ȯ = WSAEWOULDBLOCK
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}
TAsyncSelect::TAsyncSelect()
{
}


TAsyncSelect::~TAsyncSelect()
{
}
