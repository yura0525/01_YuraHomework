#pragma once
#include "TPacketPool_1.h"
#include "TProtocolChat.h"

class TChatPacketPool : public TPacketPool_1
{
public:
	void ProcessWork(T_PACKET* pUserData);
public:
	TChatPacketPool();
	~TChatPacketPool();
};

