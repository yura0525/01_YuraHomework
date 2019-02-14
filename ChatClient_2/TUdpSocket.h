#pragma once

#include "TThread.h"

class TUdpSocket :public TThread
{
public:
	TUdpSocket();
	virtual ~TUdpSocket();
};

