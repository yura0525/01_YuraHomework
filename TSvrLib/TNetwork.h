#pragma once
#include "TSvrObject.h"

class TNetwork : public TSvrObject
{
public:
	SOCKET m_ListenSock;
public:
	bool Set(int iPort, const char* address = 0);
	bool Release();

public:
	TNetwork();
	virtual ~TNetwork();
};

