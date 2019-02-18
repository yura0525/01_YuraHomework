#include "stdafx.h"
#include "TUdpSocket.h"

//IP_ADD_MEMBERSHIP		- 멀티캐스트 그룹에 가입
//IP_DROP_MEMBERSHIP	- 멀티캐스트 그룹에서 탈퇴
//IP_MULTICAST_LOOP		- 멀티캐스트 패킷의 loopback 허용 여부 지정
//IP_MULTICAST_TTL		- 멀티캐스트 패킷의 TTL 값 지정
//IP_MULTICAST_IF		- 멀티캐스트 패킷 전송용 인터페이스 지정

// 포트 번호( 0 ~ 65535 )
// 0		~ 1023	: 지정(잘알려진) 포트 번호
// 1024		~ 49151 : 등록된 포트 번호
// 49152	~ 65535 : 동적 포트 번호( 자유롭게 사용 가능 )

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
	// 멀티캐스트 그룹 탈퇴
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
