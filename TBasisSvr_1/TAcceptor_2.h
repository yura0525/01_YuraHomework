#pragma once
#include "TThread_3.h"
class TAcceptor_2 : public TThread_3
{
public:
	SOCKET			m_ListenSock;
public:
	bool			Run();
	bool			Set(int iPort, const char* strAddress = NULL);
	int				GetIPList(vector<string>& ipList);
public:
	TAcceptor_2();
	virtual ~TAcceptor_2();
};

