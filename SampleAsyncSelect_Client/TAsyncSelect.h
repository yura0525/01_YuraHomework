#pragma once
#include "TNetwork.h"
class TAsyncSelect : public TNetwork
{
public:
	bool		Connect(const char* ip, int iPort);
	void		MsgEvent(MSG msg);
public:
	TAsyncSelect();
	virtual ~TAsyncSelect();
};

