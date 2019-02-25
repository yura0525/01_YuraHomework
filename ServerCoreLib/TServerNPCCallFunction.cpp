#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"


void TServer::SpawnMonster(T_PACKET& pSendUser)
{
}

void TServer::SyncMonster(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	{
		TSynchronize sync(&m_NPCMgr);
		RecvStream >> m_NPCMgr;
	}
}

void TServer::SpawnMonsterList()
{
	stringstream data;
	{
		TSynchronize sync(&m_NPCMgr);
		data << m_NPCMgr;
	}
	stringstream stream(MakePacket(PACKET_SPAWN_CHARACTER, data));
	Broadcast(stream);
}