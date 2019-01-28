#include "TNetwork.h"

bool TNetwork::Init()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Sock)
	{
		return false;
	}
	return true;
}
bool TNetwork::Release()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}
bool TNetwork::NetworkProcess()
{
	while (1)
	{
		if (Run() == false)
		{
			break;
		}
		NetworkFrame();
	}
	return true;
}
bool TNetwork::NetworkFrame()
{
	return true;
}
bool TNetwork::Run()
{
	return true;
}
bool TNetwork::Push(UPACKET& packet)
{
	m_SendList.push_back(packet);
	return true;
}
bool TNetwork::PushMsg(char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	m_SendList.push_back(sendmsg);
	return true;
}
int	TNetwork::RecvMsg()
{
	
	int iLen = 0;
	m_dwRecvByte += recv(m_Sock, &m_szRecvBuffer[m_iStartByte], PACKET_HEADER_SIZE - m_dwRecvByte, 0);
	if (m_dwRecvByte == 0 || m_dwRecvByte == SOCKET_ERROR)
	{
		return 0;
	}
	if (m_dwRecvByte == PACKET_HEADER_SIZE)
	{
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer;
		while (m_dwRecvByte < pPacket->ph.len)
		{
			m_dwRecvByte += recv(m_Sock, &m_szRecvBuffer[m_dwRecvByte], pPacket->ph.len - m_dwRecvByte, 0);
		}

		if (m_dwRecvByte == pPacket->ph.len)
		{
			UPACKET recvmsg;
			memset(&recvmsg, 0, sizeof(recvmsg));
			memcpy(&recvmsg, pPacket, sizeof(pPacket->ph.len));
			m_RecvList.push_back(recvmsg);
			m_iStartByte = m_dwRecvByte = 0;
		}
	}
	else
	{
		m_iStartByte += m_dwRecvByte;
	}
	return 0;
}
int	TNetwork::SendMsg()
{
	return 0;
}
int	TNetwork::SendMsg(char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = send(m_Sock, (char*)&sendmsg, sendmsg.ph.len, 0);
	return iRet;
}
int	TNetwork::SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = send(sock, (char*)&sendmsg, sendmsg.ph.len, 0);

	return 0;
}

bool TNetwork::Connect(char* ip, int iPort)
{
	SOCKADDR_IN server;
	ZeroMemory(&server, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(ip);

	DWORD dwRet = connect(m_Sock,
		(sockaddr*)&server,
		sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == dwRet)
	{
		// 비동기 정상 반환 = WSAEWOULDBLOCK
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}
bool TNetwork::Broadcastting(UPACKET* pPacket)
{
	return true;
}


TNetwork::TNetwork()
{
	m_dwRecvByte;// = 0;
	m_iStartByte;// = 0;
	m_bConnect = false;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
}


TNetwork::~TNetwork()
{
}
