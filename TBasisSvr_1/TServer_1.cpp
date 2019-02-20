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

	m_fnExecutePacket[PACKET_CHAT_NAME_REQ]		= &TServer_1::ReqChatName;
	m_fnExecutePacket[PACKET_CHAT_NAME_ACK]		= &TServer_1::AckChatName;
	m_fnExecutePacket[PACKET_CHAT_MSG]			= &TServer_1::Msg;
	m_fnExecutePacket[PACKET_USER_POSITION]		= &TServer_1::MoveAction;
	m_fnExecutePacket[PACKET_LOGIN]				= &TServer_1::Login;
	m_fnExecutePacket[PACKET_LOGOUT]			= &TServer_1::Logout;
	m_fnExecutePacket[PACKET_CREATE_ACCOUNT]	= &TServer_1::CreateAccount;
	m_fnExecutePacket[PACKET_DELETE_ACCOUNT]	= &TServer_1::DeleteAccount;
	m_fnExecutePacket[PACKET_CREATE_CHARACTER]	= &TServer_1::CreateCharacter;
	m_fnExecutePacket[PACKET_DELETE_CHARACTER]	= &TServer_1::DeleteCharacter;

	m_fnExecutePacket[PACKET_HIT_CHARACTER]		= &TServer_1::HitCharacter;
	m_fnExecutePacket[PACKET_HIT_MONSTER]		= &TServer_1::HitMonster;
	m_fnExecutePacket[PACKET_ATTACK_CHARACTER]	= &TServer_1::AttackCharacter;
	m_fnExecutePacket[PACKET_ATTACK_MONSTER]	= &TServer_1::AttackMonster;
	m_fnExecutePacket[PACKET_DEAD_CHARACTER]	= &TServer_1::DeadCharacters;
	m_fnExecutePacket[PACKET_DEAD_MONSTER]		= &TServer_1::DeadMonster;
	m_fnExecutePacket[PACKET_DAMAGE_CHARACTER]	= &TServer_1::DamageCharacter;
	m_fnExecutePacket[PACKET_DAMAGE_MONSTER]	= &TServer_1::DamageMonster;
	m_fnExecutePacket[PACKET_SPAWN_CHARACTER]	= &TServer_1::SpawnCharacters;
	m_fnExecutePacket[PACKET_SPAWN_MONSTER]		= &TServer_1::SpawnMonster;
	m_fnExecutePacket[PACKET_SYNC_CHARACTER]	= &TServer_1::SyncCharacters;
	m_fnExecutePacket[PACKET_SYNC_MONSTER]		= &TServer_1::SyncMonster;

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
	if (m_Mutex != NULL)
	{
		CloseHandle(m_Mutex);
		m_Mutex = NULL;
	}
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

int	 TServer_1::SendPacket(TUser* pUser, UPACKET& PacketMsg)
{
	pUser->m_wsaBuffer.buf = (char*)&PacketMsg;
	pUser->m_wsaBuffer.len = PacketMsg.ph.len;
	pUser->m_ov.m_iFlags = OVERLAPPED2::MODE_SEND;
	DWORD dwSendByte = PacketMsg.ph.len;

	int iRet = WSASend(pUser->m_Socket, &pUser->m_wsaBuffer, 1, &dwSendByte, 0, (LPOVERLAPPED)&pUser->m_ov, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		I_DebugStr.T_ERROR();
		return 0;
	}
	return 1;
}
void TServer_1::SendPacket(TUser* pUser, int iSendByte)
{
	pUser->m_wsaBuffer.buf = pUser->m_sendBuffer;
	pUser->m_wsaBuffer.len = iSendByte;
	pUser->m_ov.m_iFlags = OVERLAPPED2::MODE_SEND;
	DWORD dwSendByte = iSendByte;

	int iRet = WSASend(pUser->m_Socket, &(pUser->m_wsaBuffer), 1, &dwSendByte, 0, (LPOVERLAPPED)&pUser->m_ov, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		I_DebugStr.T_ERROR();
	}
}
void TServer_1::Broadcast(T_PACKET& pSendUser)
{
	for (m_ListItor iter = I_Server.m_UserList.begin(); iter != I_Server.m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		if (pUser->m_Socket != pSendUser.pUser->m_Socket)
		{
			if (SendPacket(pUser, pSendUser.packet) <= 0)
				continue;
		}
	}
}
void TServer_1::Broadcast(UPACKET& packet)
{
	for (m_ListItor iter = I_Server.m_UserList.begin(); iter != I_Server.m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		if (SendPacket(pUser, packet) <= 0)
			continue;
	}
}
void TServer_1::Broadcast(stringstream& SendStream)
{
	for (auto user : m_UserList)
	{
		CopyMemory(user.second->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		SendPacket(user.second, SendStream.str().length());
	}
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

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)iter->second;
		if (pUser->m_iEvent == iIndex)
		{
			I_DebugStr.DisplayText(const_cast<char*>("\n%s%s\n"), pUser->m_Name.c_str(), "님이 나가셨습니다.");
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

	I_DebugStr.DisplayText(const_cast<char*>("\nTServer_1::MoveAction() POS->%d:%d,%d:%d\n"), pData->user_idx, pData->direction, pData->posX, pData->posY);
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
	I_DebugStr.DisplayText(const_cast<char*>("\nTServer_1::Msg() Message:%s\n"), pPacket->msg);
	m_ListItor iter;

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
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

	char buffer[64] = "님이 입장하였습니다.\n";
	strcpy(&PacketMsg.msg[pPacket->ph.len - PACKET_HEADER_SIZE], buffer);
	PacketMsg.ph.len += strlen(buffer);

	I_DebugStr.DisplayText(const_cast<char*>("\nTServer_1::AckChatName() Message:%s\n"), PacketMsg.msg);

	Broadcast(PacketMsg);
}
void TServer_1::ReqChatName(T_PACKET& pSendUser) {}

void TServer_1::CreateAccount(T_PACKET& pSendUser)
{
	tGUID	uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
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
