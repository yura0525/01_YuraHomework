#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"

void TServer::ReqZoneEntry(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	{
		TSynchronize sync(this);
		TCharacter tChar;
		RecvStream >> tChar;
		// superuser check
		if (m_UserCharacterList.Get().size() == 0)
		{
			tChar.m_Info.iType = 1;
			pSendUser.pUser->m_iType = 1;
		}
		pSendUser.pUser->GetCharacterList()[tChar.m_Info.tGuid] = tChar;
		m_UserCharacterList.Get()[tChar.m_Info.tGuid] = tChar;				//active character list
		AckZoneEntry(pSendUser);
		BroadcastNewPlayer(tChar);
		pSendUser.pUser->m_ConnectType = CONNECT_TYPE::eGAMEPLAYING;
	}
	I_DebugStr.DisplayText("\nTServer::ReqZoneEntry() PACKET_ZONE_ENTRY_REQ\n");
}
void TServer::AckZoneEntry(T_PACKET& pSendUser)
{
	if (pSendUser.pUser->m_iType == 1)
	{
		stringstream data;
		stringstream stream(MakePacket(PACKET_ZONE_TARGET_REQ, data));
		SendPacket(pSendUser.pUser, stream);
	}
	I_DebugStr.DisplayText("\nTServer::AckZoneEntry() PACKET_ZONE_TARGET_REQ\n");
}

void TServer::RecvZoneTargetPoint(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iNumCount = 0;
	tBlockRead<int>(RecvStream, iNumCount);

	m_tpMgr.Get().clear();
	m_NPCMgr.Clear();

	TPACKET_TARGETPOINT_INFO tp;
	for (int i = 0; i < iNumCount; i++)
	{
		tBlockRead<TPACKET_TARGETPOINT_INFO>(RecvStream, tp);
		m_tpMgr.Get()[i] = tp;
		m_NPCMgr.Add(tp.vPosition);
	}
	m_NPCMgr.SyncNPC();
	I_DebugStr.DisplayText("\nTServer::RecvZoneTargetPoint() PACKET_ZONE_TARGET_ACK\n");
}