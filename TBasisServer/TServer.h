#pragma once
#include "TThread.h"
#include "TAcceptor.h"

class TServer : public TThread
{
public:
	TAcceptor	m_Acceptor;
public:
	bool Init();
	bool Release();
	bool Run();
	virtual bool AddUser(SOCKET socket, SOCKADDR_IN address);
	bool SendMsg(SOCKET socket, char* msg, WORD code);
public:
	TServer();
	virtual ~TServer();
};

