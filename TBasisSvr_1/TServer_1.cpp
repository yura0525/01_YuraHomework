#define _CRT_SECURE_NO_WARNINGS
#include "TServer_1.h"
#include "TDebugString_2.h"
#include "TServerIOCP_1.h"
#include "TSynchronize_1.h"

bool TServer_1::Init()
{
	int iRet;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	m_fnExecutePacket[PACKET_CHAT_NAME_REQ] = &TServer::ReqChatName;
	m_fnExecutePacket[PACKET_CHAT_NAME_ACK] = &TServer::AckChatName;
	m_fnExecutePacket[PACKET_CHAT_MSG] = &TServer::Msg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &TServer::MoveAction;
	m_fnExecutePacket[PACKET_LOGIN] = &TServer::Login;
	m_fnExecutePacket[PACKET_LOGIOUT] = &TServer::Logout;
	m_fnExecutePacket[PACKET_CREATE_ACCOUNT] = &TServer::CreateAccount;
	m_fnExecutePacket[PACKET_DELETE_ACCOUNT] = &TServer::DeleteAccount;
	m_fnExecutePacket[PACKET_CREATE_CHARACTER] = &TServer::CreateCharacter;
	m_fnExecutePacket[PACKET_DELETE_CHARACTER] = &TServer::DeleteCharacter;

	m_fnExecutePacket[PACKET_HIT_CHARACTER] = &TServer::HitCharacter;
	m_fnExecutePacket[PACKET_HIT_MONSTER] = &TServer::HitMonster;
	m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = &TServer::AttackCharacter;
	m_fnExecutePacket[PACKET_ATTACK_MONSTER] = &TServer::AttackMonster;
	m_fnExecutePacket[PACKET_DEAD_CHARACTER] = &TServer::DeadCharacters;
	m_fnExecutePacket[PACKET_DEAD_MONSTER] = &TServer::DeadMonster;
	m_fnExecutePacket[PACKET_DAMAGE_CHARACTER] = &TServer::DamageCharacter;
	m_fnExecutePacket[PACKET_DAMAGE_MONSTER] = &TServer::DamageMonster;
	m_fnExecutePacket[PACKET_SPAWN_CHARACTER] = &TServer::SpawnCharacters;
	m_fnExecutePacket[PACKET_SPAWN_MONSTER] = &TServer::SpawnMonster;
	m_fnExecutePacket[PACKET_SYNC_CHARACTER] = &TServer::SyncCharacters;
	m_fnExecutePacket[PACKET_SYNC_MONSTER] = &TServer::SyncMonster;

	// IOCP 생성
	I_ServerIOCP.Init();
	m_Mutex = CreateMutex(NULL, FALSE, _T("EditMutex"));

	// Accept 처리
	if (!m_Acceptor.Set(10000))
	{
		return false;
	}
	return true;
}
bool TServer_1::Frame()
{
	return true;
}
bool TServer_1::Render()
{
	return true;
}
bool TServer_1::Release()
{
	WSACleanup();
	m_ListItor	iter;
	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		SAFE_DEL(pUser);
	}

	m_UserList.clear();
	CloseHandle(m_Mutex);
	m_Mutex = NULL;

	return true;
}
void TServer_1::ProcessPacket()
{
	WaitForSingleObject(m_Mutex, INFINITE);

	int retval;
	std::list<T_PACKET>::iterator iter;
	for (iter = m_StreamPacket.m_PacketList.begin(); iter != m_StreamPacket.m_PacketList.end(); iter++)
	{
		UPACKET PacketMsg;
		ZeroMemory(&PacketMsg, sizeof(PacketMsg));

		T_PACKET pSendUser = (*iter);
		std::map<int, CallFunction>::iterator calliter = m_fnExecutePacket.find(pSendUser.packet.ph.type);
		if (calliter != m_fnExecutePacket.end())
		{
			CallFunction call = calliter->second;
			(this->*call)(*iter);
		}
	}
	m_StreamPacket.m_PacketList.clear();
	ReleaseMutex(m_Mutex);
	return;
}

void	TServer_1::AddPacket(T_PACKET& pack)
{
	WaitForSingleObject(m_Mutex, INFINITE);
		m_StreamPacket.m_PacketList.push_back(pack);
	ReleaseMutex(m_Mutex);
}
TUser* TServer_1::GetUser(int iIndex)
{
	WaitForSingleObject(m_Mutex, INFINITE);
	m_ListItor	iter;

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		if (pUser->m_iEvent == iIndex)
		{
			ReleaseMutex(m_Mutex);
			return pUser;
		}
	}

	ReleaseMutex(m_Mutex);
	return NULL;
}
bool TServer_1::DelUser(int iIndex)
{
	// 방에서 나감
	WaitForSingleObject(m_Mutex, INFINITE);
	m_ListItor	iter;
	m_ListItor delUser = m_UserList.end();

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++;)
	{
		TUser* pUser = (TUser*)iter->second;
		if (pUser->m_iEvent == iIndex)
		{
			I_DebugStr.DisplayText(const_cast<char*>("%s%s\r\n"), pUser->m_Name.c_str(), "님이 나가셨습니다.");
			closesocket(pUser->m_Socket);
			m_UserList.erase(iter);
			break;
		}
	}

	ReleaseMutex(m_Mutex);
	return true;
}

void TServer_1::MoveAction(T_PACKET& pSendUser)
{
	UPACKET* pPacket = &(pSendUser.packet);
	TPACKET_USER_POSITION* pData = (TPACKET_USER_POSITION*)pPacket->msg;

	I_DebugStr.DisplayText(const_cast<char*>("POS->%d:%d,%d:%d\r\n"), pData->user_idx, pData->direction,
		pData->posX, pData->posY);
	m_ListItor iter;
	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		{
			if (SendPacket(pUser, pSendUser.packet) <= 0)
				break;
		}
	}
}
void TServer_1::Msg(T_PACKET& pSendUser)
{
	UPACKET* pPacket = &(pSendUser.packet);
	I_DebugStr.DisplayText(const_cast<char*>("Message:%s\r\n"), pPacket->msg);
	m_ListItor iter;
	int iClientUser = I_Server.m_UserList.size();
	for (iter = I_Server.m_UserList.begin();
		iter != I_Server.m_UserList.end();
		iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		if (pUser->m_Socket != pSendUser.pUser->m_Socket)
		{
			if (SendPacket(pUser, pSendUser.packet) <= 0)
			{
				break;
			}
		}
	}
}
void TServer_1::AckChatName(T_PACKET& pSendUser)
{
	UPACKET* pPacket = &(pSendUser.packet);

	UPACKET PacketMsg;
	ZeroMemory(&PacketMsg, sizeof(PacketMsg));

	PacketMsg.ph.len = pPacket->ph.len;
	PacketMsg.ph.type = PACKET_CHAT_MSG;

	pPacket->msg[pPacket->ph.len - PACKET_HEADER_SIZE] = 0;
	C_STR name = pPacket->msg;
	memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len - PACKET_HEADER_SIZE);

	char buffer[64] = "님이 입장하였습니다.";
	strcpy(&PacketMsg.msg[pPacket->ph.len - PACKET_HEADER_SIZE], buffer);
	PacketMsg.ph.len += strlen(buffer);

	I_DebugStr.DisplayText(const_cast<char*>("Message:%s\r\n"), PacketMsg.msg);

	Broadcast(PacketMsg);
}
void TServer_1::ReqChatName(T_PACKET& pSendUser) {}

void TServer_1::CreateAccount(T_PACKET& pSendUser)
{
	tGUID	uuid;
	RPC_STATUS ret_val = ::UuidCreate(uuid);
}
void TServer_1::DeleteAccount(T_PACKET& pSendUser) {}

void TServer_1::Login(T_PACKET& pSendUser) {}

void TServer_1::Logout(T_PACKET& pSendUser) {}

void TServer_1::CreateCharacter(T_PACKET& pSendUser) {}

void TServer_1::DeleteCharacter(T_PACKET& pSendUser) {}

void TServer_1::HitCharacter(T_PACKET& pSendUser) {}

void TServer_1::HitMonster(T_PACKET& pSendUser) {}

void TServer_1::AttackCharacter(T_PACKET& pSendUser) {}

void TServer_1::AttackMonster(T_PACKET& pSendUser) {}

void TServer_1::DeadCharacters(T_PACKET& pSendUser) {}

void TServer_1::DeadMonster(T_PACKET& pSendUser) {}

void TServer_1::DamageCharacter(T_PACKET& pSendUser) {}

void TServer_1::DamageMonster(T_PACKET& pSendUser) {}

void TServer_1::SpawnCharacters(T_PACKET& pSendUser) {}

void TServer_1::SpawnMonster(T_PACKET& pSendUser) {}

void TServer_1::SyncCharacters(T_PACKET& pSendUser) {}

void TServer_1::SyncMonster(T_PACKET& pSendUser) {}

TServer_1::TServer_1()
{
}


TServer_1::~TServer_1()
{
}
