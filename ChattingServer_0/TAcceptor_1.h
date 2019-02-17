#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TThread_1.h"
#define BUFFERSIZE 1024
#define NAMESIZE 30

class TAcceptor_1 : public TThread_1
{
public:
	SOCKET			m_ListenSock;
public:
	bool			Run();
	bool			Set(int iPort, const char* strAddress = 0);
	int				GetIPList(vector<string>& ipList);
	bool			Release();
public:
	TAcceptor_1();
	virtual ~TAcceptor_1();
};

