#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"

void TServer::SyncAllCharacters(TUser* pUser)
{
	{
		TSynchronize sync(this);
		stringstream data;
		data << m_UserCharacterList;

		stringstream stream(MakePacket(PACKET_ZONE_PLAYERS_INFO, data));
		SendPacket(pUser, stream);
	}
}
void TServer::SendDeadPlayer(TUser* pUser)
{
	{
		TSynchronize sync(this);
		stringstream data;
		data << pUser->m_GuidActiveCharacter;

		stringstream stream(MakePacket(PACKET_DEAD_CHARACTER, data));
		SendPacket(pUser, stream);
		m_UserCharacterList.Get()[pUser->m_GuidActiveCharacter].m_Info.bAlive = false;
	}
}

void TServer::MoveAction(T_PACKET& pSendUser)
{
	{
		TSynchronize sync(this);
		UPACKET* pPacket = &(pSendUser.packet);
		TPACKET_USER_POSITION* pData = (TPACKET_USER_POSITION*)pPacket->msg;

		I_DebugStr.DisplayText("\nTServer::MoveAction() POS->%d:%d,%d:%d\n", pData->user_idx, pData->direction, pData->posX, pData->posY);
		TUserListItor iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, pSendUser.packet) <= 0)
				break;
		}
	}
}

void TServer::CreateCharacter(T_PACKET& pSendUser)
{
	tGUID uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int type;
	string name;
	RecvStream >> type;
	RecvStream >> name;

	stringstream data;
	data << true;
	data << uuid;
	data << type;
	data << name << endl;
	stringstream stream(MakePacket(PACKET_CREATE_ACCOUNT_ACK, data));

	SendPacket(pSendUser.pUser, stream);
}

void TServer::SpawnCharacter(T_PACKET& pSendUser)
{
	//stringstream data;
	//data << m_NPCMgr;
	//stringstream stream(MakePacket(PACKET_SPAWN_CHARACTER, data));

	//SendPacket(pSendUser.pUser, stream);
}

void TServer::SyncCharacters(T_PACKET& pSendUser)
{
	{
		//I_DebugStr.DisplayText("\nTServer::SyncCharacters() PACKET_SYNC_CHARACTER Start\n");	
		TSynchronize sync(this);
		
		string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
		stringstream RecvStream(msg);
		TCharacter tChar;
		RecvStream >> tChar;
		tChar.m_Info.fHealth = m_UserCharacterList.Get()[tChar.m_Info.tGuid].m_Info.fHealth;
		if (tChar.m_Info.fHealth <= 0.0f)
		{
			tChar.m_Info.bAlive = false;
		}
		m_UserCharacterList.Get()[tChar.m_Info.tGuid] = tChar;
	}
	SyncAllCharacters(pSendUser.pUser);
}



void TServer::DeleteCharacter(T_PACKET& pSendUser) {}
void TServer::HitCharacter(T_PACKET& pSendUser) {}
void TServer::HitMonster(T_PACKET& pSendUser) {}
void TServer::AttackCharacter(T_PACKET& pSendUser) {}
void TServer::AttackMonster(T_PACKET& pSendUser) {}
void TServer::DeadCharacter(T_PACKET& pSendUser) {}
void TServer::DeadMonster(T_PACKET& pSendUser) {}
void TServer::DamageCharacter(T_PACKET& pSendUser) {}
void TServer::DamageMonster(T_PACKET& pSendUser) {}