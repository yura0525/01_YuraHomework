#pragma once
#include "TServerStd.h"
#include "TThread.h"
#include "TServer.h"
#include <WS2tcpip.h>

class TUdpSocket_3 : public TThread
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

	TUdpSocket_3();
	~TUdpSocket_3();
};

