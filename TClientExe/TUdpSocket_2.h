#pragma once
#include "TBasisStd.h"
#include "TThread_2.h"

class TUdpSocket_2 : public TThread_2
{
public:
	SOCKET			m_Socket;
	SOCKADDR_IN		m_SenderAddr;
	SOCKADDR_IN		m_RecvAddr;
	ip_mreq			m_MemberReq;
public:
	bool		Init();
	bool		Run();
	bool		Release();
public:
	TUdpSocket_2(void);
	~TUdpSocket_2(void);
};
