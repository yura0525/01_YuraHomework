#include "TPacketPool_1.h"

bool TPacketPool_1::AddPacket(T_PACKET& addPacket)
{
	{
		TSynchronize sync(this);
		m_PacketList.push_back(addPacket);
	}
	return true;
}
bool TPacketPool_1::ProcessPacket()
{
	{
		TSynchronize sync(this);
		std::list<T_PACKET>::iterator iter;
		for (iter = m_PacketList.begin(); iter != m_PacketList.end(); iter++)
		{
			T_PACKET* pUserData = (T_PACKET*)&(*iter);
			ProcessWork(pUserData);
		}
		m_PacketList.clear();
	}
	return true;
}
void TPacketPool_1::ProcessWork(T_PACKET* pUserData)
{

}

TPacketPool_1::TPacketPool_1()
{
}


TPacketPool_1::~TPacketPool_1()
{
}
