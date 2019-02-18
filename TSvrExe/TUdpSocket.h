#pragma once
#include "TServerStd.h"
#include <WS2tcpip.h>
#include "TThread_3.h"

class TUdpSocket : public TThread_3
{
public:
	SOCKET			m_Socket;
	SOCKADDR_IN		m_RemoteAddr;
	SOCKADDR_IN		m_RecvAddr;
	ip_mreq			m_MemberReq;
public:
	bool			Init();
	bool			Run();
	bool			Release();
public:
	TUdpSocket();
	~TUdpSocket();
};

