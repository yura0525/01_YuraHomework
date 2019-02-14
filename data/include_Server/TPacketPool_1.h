#pragma once
#include "TThread.h"
#include "TStreamPacket_1.h"

class TPacketPool_1 : public TServerObj
{
public:
	std::list<T_PACKET>	m_PacketList;
	typedef std::list<T_PACKET>::iterator m_PackItor;
public:
	virtual bool	AddPacket(T_PACKET& addPacket);
	virtual bool	ProcessPacket();
	virtual void	ProcessWork(T_PACKET* pUserData);


public:
	TPacketPool_1();
	virtual ~TPacketPool_1();
};

