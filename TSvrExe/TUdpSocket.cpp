#include "stdafx.h"
#include "TUdpSocket.h"

//IP_ADD_MEMBERSHIP		- ��Ƽĳ��Ʈ �׷쿡 ����
//IP_DROP_MEMBERSHIP	- ��Ƽĳ��Ʈ �׷쿡�� Ż��
//IP_MULTICAST_LOOP		- ��Ƽĳ��Ʈ ��Ŷ�� loopback ��� ���� ����
//IP_MULTICAST_TTL		- ��Ƽĳ��Ʈ ��Ŷ�� TTL �� ����
//IP_MULTICAST_IF		- ��Ƽĳ��Ʈ ��Ŷ ���ۿ� �������̽� ����

// ��Ʈ ��ȣ( 0 ~ 65535 )
// 0		~ 1023	: ����(�߾˷���) ��Ʈ ��ȣ
// 1024		~ 49151 : ��ϵ� ��Ʈ ��ȣ
// 49152	~ 65535 : ���� ��Ʈ ��ȣ( �����Ӱ� ��� ���� )

bool TUdpSocket::Init()
{
	int retval;
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	int ttl = 64;
	retval = setsockopt(m_Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));

}
bool TUdpSocket::Run()
{

}
bool TUdpSocket::Release()
{
	// ��Ƽĳ��Ʈ �׷� Ż��
	int retval = setsockopt(m_Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_MemberReq, sizeof(m_MemberReq));
	if (retval == SOCKET_ERROR)
	{
		I_DebugStr.T_ERROR(false);
	}
	closesocket(m_Socket);
	return true;
}

TUdpSocket::TUdpSocket()
{
}


TUdpSocket::~TUdpSocket()
{
}
