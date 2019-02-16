#pragma once
#include "TThread_1.h"
#define BUFFERSIZE 1024
#define NAMESIZE 30

class TAcceptor_1 : public TThread_1
{
public:
	SOCKET			m_ListenSock;
public:
	bool			Init();
	bool			Run();
	bool			Set(int iPort, const char* strAddress = 0);
	int				GetIPList();
	bool			Release();
public:
	TAcceptor_1();
	virtual ~TAcceptor_1();
};

