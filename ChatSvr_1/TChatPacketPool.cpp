#include "TChatPacketPool.h"
#include "TChatUser.h"
#include "TDebugString.h"
#include "TChatSvr.h"

void TChatPacketPool::ProcessWork(T_PACKET* pUserData)
{
	UPACKET*	pPacket	= &(pUserData->packet);
	TChatUser*	pUser	= (TChatUser*)pUserData->pUser;
	switch (pPacket->ph.type)
	{
	case PACKET_USER_POSITION:
	{
		pUser->Movement(pPacket);
		I_Server.Broadcastting(pPacket, pUser->m_Socket);
	}
	break;
	case PACKET_CHAT_NAME_REQ:
	{
		pUser->m_szName = pPacket->msg;
		char buffer[256] = { 0, };
		sprintf_s(buffer, "[%s]%s", pUser->m_szName.c_str(), "���� �����Ͽ����ϴ�");
		ZeroMemory(pPacket->msg, 2048);
		sprintf_s(pPacket->msg, "%s", buffer);
		pPacket->ph.len = sizeof(PACKET_HEADER) + strlen(buffer);
		pPacket->ph.type = PACKET_CHAT_MSG;

		I_Server.Broadcastting(pPacket, pUser->m_Socket);
	}
	break;
	case PACKET_CHAT_MSG:
	{
		I_Server.Broadcastting(pPacket, pUser->m_Socket);
		break;
	}
	}
}

TChatPacketPool::TChatPacketPool()
{
}


TChatPacketPool::~TChatPacketPool()
{
}
