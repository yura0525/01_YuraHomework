#define _CRT_SECURE_NO_WARNINGS
#include "TUdpSocket_2.h"
#include "TDebugString_1.h"

// ��Ƽĳ��Ʈ(udp)�� ����ͷ� ��Ƽĳ��Ʈ ���� �����ϸ� 
//                   ���� ���� �����Ͽ� ��Ƽ ĳ��Ʈ�� ����
// �޴� ��ǻ�ʹ� �ڽ��� iP�� ��Ƽĳ��Ʈ�� ����ϴ� ������ �ʿ�
bool TUdpSocket_2::Init()
{
	int retval;
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

	// ��Ƽĳ��Ʈ TTL ����
	int ttl = 64;// Time To Live
	retval = setsockopt(m_Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) {
		I_DebugStr.T_ERROR(false);
	}
	BOOL optval = TRUE;
	retval = setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		I_DebugStr.T_ERROR(false);
	}
	// Recv	: Port��ȣ�� �޸� �׷�ȭ�ϸ� Channel�� ����� ����.
	ZeroMemory(&m_RecvAddr, sizeof(m_RecvAddr));
	m_RecvAddr.sin_family = AF_INET;
	m_RecvAddr.sin_port = htons(10000);
	m_RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = ::bind(m_Socket, (SOCKADDR *)&m_RecvAddr, sizeof(m_RecvAddr));
	if (retval == SOCKET_ERROR)
	{
		I_DebugStr.T_ERROR(false);
	}
	// Send
	ZeroMemory(&m_SenderAddr, sizeof(m_SenderAddr));
	m_SenderAddr.sin_family = AF_INET;
	m_SenderAddr.sin_port = htons(9001);
	m_SenderAddr.sin_addr.s_addr = inet_addr("235.7.8.9");//inet_addr("127.0.0.1");//htonl(INADDR_BROADCAST);

	// ����
	// �׷쿡 ������ ȣ��Ʈ �ּ�.
	m_MemberReq.imr_interface.s_addr = htonl(INADDR_ANY);
	// ��Ƽĳ��Ʈ �׷��� �ּ�
	m_MemberReq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	retval = setsockopt(m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&m_MemberReq, sizeof(m_MemberReq));
	if (retval == SOCKET_ERROR)
	{
		I_DebugStr.T_ERROR();
	}

	CreateThread();

	return true;
}
bool TUdpSocket_2::Run()
{
	int retval;
	int addrlen;
	SOCKADDR_IN peeraddr;
	static int iCnt = 0;
	char buf[2048] = { 0, };
	while (1)
	{
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "[UDP][%d]ChattingMsg[1Sec Per Msg]----------------", iCnt++);
		retval = sendto(m_Socket, buf, strlen(buf), 0, (SOCKADDR*)&m_SenderAddr, sizeof(m_SenderAddr));
		if (retval == SOCKET_ERROR)
		{
			I_DebugStr.T_ERROR(false);
		}
		addrlen = sizeof(peeraddr);
		retval = recvfrom(m_Socket, buf, 2048, 0, (SOCKADDR*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR)
		{
			I_DebugStr.T_ERROR(false);
		}
		else
		{
			buf[retval] = 0;
			I_DebugStr.DisplayText(const_cast<char*>("[MSG]:%s"), buf);
		}
		Sleep(1000);
	}

	return true;
}
bool TUdpSocket_2::Release()
{
	// ��Ƽĳ��Ʈ �׷� Ż��
	int retval = setsockopt(m_Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_MemberReq, sizeof(m_MemberReq));
	if (retval == SOCKET_ERROR) {
		I_DebugStr.T_ERROR(false);
	}
	closesocket(m_Socket);
	return true;
}

TUdpSocket_2::TUdpSocket_2()
{
}


TUdpSocket_2::~TUdpSocket_2()
{
}
