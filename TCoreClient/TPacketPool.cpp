#include "TPacketPool.h"


bool TPacketPool::AddPacket(UPACKET addPacket)
{
	m_RecvPacketList.push_back(addPacket);
	//printf("\nTPacketPool::AddPacket() m_RecvPacketList.push_back[%s]", addPacket.msg);
	return true;
}
bool TPacketPool::ProcessPacket()
{
	return true;
}
bool TPacketPool::ProcessWork(UPACKET addPacket)
{
	return true;
}


TPacketPool::TPacketPool()
{
}


TPacketPool::~TPacketPool()
{
}
