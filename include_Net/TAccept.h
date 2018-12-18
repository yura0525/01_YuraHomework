#pragma once
#include "TThread.h"
#include "TNetwork.h"
#include "XUser.h"

class TAccept : public TThread
{
public:
	TNetwork m_network;
public:
	bool Set(int iPort, const char* address = NULL);
	void Run();
	virtual XUser* AddUser(SOCKET clientSocket, SOCKADDR_IN clientAddr);
public:
	TAccept();
	virtual ~TAccept();
};

