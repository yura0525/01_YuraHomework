#pragma once
#include "TThread.h"
#include <WS2tcpip.h>

class TUdpSocket_1 : public TThread
{
public:
	SOCKET			m_Socket;
	SOCKET			m_SendSocket;
	SOCKADDR_IN		m_RecvAddr;
	SOCKADDR_IN		m_SendAddr;
public:
	bool			Init();
	bool			Run();
	bool			Release();
public:
	TUdpSocket_1();
	virtual ~TUdpSocket_1();
};

