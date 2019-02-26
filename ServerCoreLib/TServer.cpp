#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"

TAcceptor& TServer::GetAcceptor()
{
	return m_Acceptor;
}
TTargetPointMgr& TServer::GetTargetPointMgr()
{
	return m_tpMgr;
}
TActiveCharacter& TServer::GetActiveCharacterList()			// active
{
	return m_UserCharacterList;
}
TServer* TServer::GetPtr()
{
	return this;
}

bool TServer::Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	m_fnExecutePacket[PACKET_VERIFICATION_REQ] = &TServer::ReqVerification;
	m_fnExecutePacket[PACKET_CHAT_NAME_REQ] = &TServer::ReqChatName;
	m_fnExecutePacket[PACKET_CHAT_NAME_ACK] = &TServer::AckChatName;
	m_fnExecutePacket[PACKET_CHAT_MSG] = &TServer::Msg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &TServer::MoveAction;
	m_fnExecutePacket[PACKET_STOP_POSITION] = &TServer::MoveAction;

	m_fnExecutePacket[PACKET_LOGIN_REQ] = &TServer::LogIn;
	m_fnExecutePacket[PACKET_LOGOUT_REQ] = &TServer::LogOut;
	m_fnExecutePacket[PACKET_CREATE_ACCOUNT_REQ] = &TServer::CreateAccount;
	m_fnExecutePacket[PACKET_DELETE_ACCOUNT_REQ] = &TServer::DeleteAccount;
	m_fnExecutePacket[PACKET_CREATE_CHARACTER_REQ] = &TServer::CreateCharacter;
	m_fnExecutePacket[PACKET_DELETE_CHARACTER_REQ] = &TServer::DeleteCharacter;
	m_fnExecutePacket[PACKET_ZONE_ENTRY_REQ] = &TServer::ReqZoneEntry;
	m_fnExecutePacket[PACKET_ZONE_TARGET_ACK] = &TServer::RecvZoneTargetPoint;

	m_fnExecutePacket[PACKET_HIT_CHARACTER] = &TServer::HitCharacter;
	m_fnExecutePacket[PACKET_HIT_MONSTER] = &TServer::HitMonster;
	m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = &TServer::AttackCharacter;
	m_fnExecutePacket[PACKET_ATTACK_MONSTER] = &TServer::AttackMonster;
	m_fnExecutePacket[PACKET_DEAD_CHARACTER] = &TServer::DeadCharacter;
	m_fnExecutePacket[PACKET_DEAD_MONSTER] = &TServer::DeadMonster;
	m_fnExecutePacket[PACKET_DAMAGE_CHARACTER] = &TServer::DamageCharacter;
	m_fnExecutePacket[PACKET_DAMAGE_MONSTER] = &TServer::DamageMonster;
	m_fnExecutePacket[PACKET_SPAWN_CHARACTER] = &TServer::SpawnCharacter;
	m_fnExecutePacket[PACKET_SYNC_CHARACTER] = &TServer::SyncCharacters;
	m_fnExecutePacket[PACKET_SYNC_MONSTER] = &TServer::SyncMonster;

	// IOCP 생성
	I_ServerIOCP.Init();
	m_hMutexPacket = CreateMutex(NULL, FALSE, _T("EditMutex"));

	// Accept 처리
	if (!m_Acceptor.Set(10000))
		return false;

	m_NPCMgr.Set();
	return true;
}
bool TServer::Frame()
{
	return true;
}
bool TServer::Render()
{
	return true;
}
bool TServer::Release()
{
	WSACleanup();
	{
		TSynchronize sync(this);
		TUserListItor iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			SAFE_DEL(pUser);
		}
		m_UserList.clear();
	}
	CloseHandle(m_hMutexPacket);
	m_hMutexPacket = NULL;
	return true;
}
void TServer::ProcessPacket()
{
	WaitForSingleObject(m_hMutexPacket, INFINITE);

	std::list<T_PACKET>::iterator iter;
	for (iter = m_StreamPacket.m_PacketList.begin();
		iter != m_StreamPacket.m_PacketList.end();
		iter++)
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
	ReleaseMutex(m_hMutexPacket);
}
void TServer::AddPacket(T_PACKET& pack)
{
	WaitForSingleObject(m_hMutexPacket, INFINITE);
		m_StreamPacket.m_PacketList.push_back(pack);
	ReleaseMutex(m_hMutexPacket);
}
int	 TServer::SendPacket(TUser* pUser, UPACKET& packetMsg)
{
	pUser->m_wsaRecvBuffer.buf = (char*)&packetMsg;
	pUser->m_wsaRecvBuffer.len = packetMsg.ph.len;
	pUser->m_OV.m_iFlags = OVERLAPPED2::MODE_SEND;
	DWORD dwSendByte = packetMsg.ph.len;

	if (pUser->m_Socket == INVALID_SOCKET)
		return 0;

	int iRet = WSASend(pUser->m_Socket, &pUser->m_wsaRecvBuffer, 1, &dwSendByte, 0, (LPOVERLAPPED)&pUser->m_OV, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		I_DebugStr.T_ERROR();
		return 0;
	}
	return 1;
}
int	 TServer::SendPacket(TUser* pUser, stringstream& SendStream)
{
	CopyMemory(pUser->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pUser->m_wsaRecvBuffer.buf = pUser->m_sendBuffer;
	pUser->m_wsaRecvBuffer.len = (ULONG)SendStream.str().length();
	pUser->m_OV.m_iFlags = OVERLAPPED2::MODE_SEND;
	DWORD dwSendByte = (DWORD)SendStream.str().length();

	if (pUser->m_Socket == INVALID_SOCKET)
		return 0;

	int iRet = WSASend(pUser->m_Socket, &pUser->m_wsaRecvBuffer, 1, &dwSendByte, 0, (LPOVERLAPPED)&pUser->m_OV, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		I_DebugStr.T_ERROR();
		return 0;
	}
	return 1;
}
void TServer::SendPacket(TUser* pUser, size_t iSendByte)
{
	pUser->m_wsaRecvBuffer.buf = pUser->m_sendBuffer;
	pUser->m_wsaRecvBuffer.len = (ULONG)iSendByte;
	pUser->m_OV.m_iFlags = OVERLAPPED2::MODE_SEND;
	DWORD dwSendByte = (DWORD)iSendByte;

	if (pUser->m_Socket == INVALID_SOCKET)
		return;

	int iRet = WSASend(pUser->m_Socket, &pUser->m_wsaRecvBuffer, 1, &dwSendByte, 0, (LPOVERLAPPED)&pUser->m_OV, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		I_DebugStr.T_ERROR();
	}
}
void TServer::Broadcast(T_PACKET& pSendUser)
{
	{
		TSynchronize sync(this);
		for (TUserListItor iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (pUser->m_Socket != pSendUser.pUser->m_Socket)
			{
				if (SendPacket(pUser, pSendUser.packet) <= 0)
					continue;
			}
		}
	}
}
void TServer::Broadcast(UPACKET& pSendUser)
{
	{
		TSynchronize sync(this);
		for (TUserListItor iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, pSendUser) <= 0)
			{
				continue;
			}
		}
	}
}
void TServer::Broadcast(stringstream& SendStream)
{
	{
		TSynchronize sync(this);
		for (auto user : m_UserList)
		{
			CopyMemory(user.second->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
			SendPacket(user.second, SendStream.str().length());
		}
	}
}
void TServer::Broadcast(tGUID tGuidExcept, stringstream& SendStream)
{
	{
		TSynchronize sync(this);
		for (auto user : m_UserList)
		{
			if (tGuidExcept != user.second->m_GuidActiveCharacter)
			{
				CopyMemory(user.second->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
				SendPacket(user.second, SendStream.str().length());
			}
		}
	}
}

void TServer::BroadcastNewPlayer(TCharacter& player)
{
	stringstream data;
	data << player;
	std::stringstream stream = MakePacket(PACKET_ZONE_NEW_PLAYER, data);
	Broadcast(stream);
	I_DebugStr.DisplayText("\nPACKET_ZONE_NEW_PLAYER Send\n");
}
TUser* TServer::GetUser(int iIndex)
{
	{
		TSynchronize sync(this);
		TUserListItor iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (pUser->m_Socket == iIndex)
			{
				//TODO:왜 릴리즈 뮤텍스해주는지 모르겠음.
				//ReleaseMutex(m_hMutexPacket);
				return pUser;
			}
		}
	}

	return NULL;
}
bool TServer::DelUser(int iIndex)
{
	{
		TSynchronize sync(this);
		TUserListItor iter;
		TUserListItor delUser = m_UserList.end();
		bool bSuperUserChange = false;

		for (iter = m_UserList.begin(); iter != m_UserList.end(); )
		{
			TUser* pUser = (TUser*)iter->second;
			if (pUser->m_Socket == iIndex)
			{
				if (pUser->m_iType == 1)
					bSuperUserChange = true;

				I_DebugStr.DisplayText("\n%s%s\n", pUser->m_Name.c_str(), "님이 나가셨습니다.");
				SendLogoutPlayer(pUser->m_GuidActiveCharacter);
				closesocket(pUser->m_Socket);

				m_UserCharacterList.Get().erase(pUser->m_GuidActiveCharacter);
				SAFE_DEL(pUser);
				m_UserList.erase(iter);
				I_DebugStr.DisplayText("\nTServer::DelUser() %s%d\n", "m_UserList.size() Count=", m_UserList.size());
				I_DebugStr.DisplayText("\nTServer::DelUser() %s%d\n", "m_UserCharacterList.size() Count=", m_UserCharacterList.Get().size());
				break;
			}
			iter++;
		}
		if (bSuperUserChange)
		{
			ChangeSuperUser();
		}
	}
	return true;
}
bool TServer::DelUser(tGUID tGuid)
{
	{
		TSynchronize sync(this);
		TUserListItor delUser = m_UserList.find(tGuid);
		bool bSuperUserChange = false;

		if(delUser != m_UserList.end())
		{
			TUser* pUser = (TUser*)delUser->second;
			if (pUser)
			{
				if (pUser->m_iType == 1)
					bSuperUserChange = true;

				I_DebugStr.DisplayText("\n%s%s\n", pUser->m_Name.c_str(), "님이 나가셨습니다.");
				SendLogoutPlayer(pUser->m_GuidActiveCharacter);
				closesocket(pUser->m_Socket);
				m_UserCharacterList.Get().erase(pUser->m_GuidActiveCharacter);
				SAFE_DEL(pUser);
			}
			m_UserList.erase(delUser);
			I_DebugStr.DisplayText("\nTServer::DelUser() %s%d\n", "m_UserList.size() Count=", m_UserList.size());
			I_DebugStr.DisplayText("\nTServer::DelUser() %s%d\n", "m_UserCharacterList.size() Count=", m_UserCharacterList.Get().size());
		}
		if (bSuperUserChange)
		{
			ChangeSuperUser();
		}
	}
	return true;
}
void TServer::AddUser(SOCKET sock)
{
	TUser* pUser = NULL;
	SAFE_NEW(pUser, TUser);
	pUser->m_Socket = sock;
	ZeroMemory(&(pUser->m_OV), sizeof(pUser->m_OV));

	tGUID uuid;
	tGUID uuidLogin;
	tGUID uuidCharacter;

	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	ret_val = ::UuidCreate(&uuidLogin);
	ret_val = ::UuidCreate(&uuidCharacter);

	// 로그인 검증 GUID
	pUser->m_LoginGuid = uuidLogin;
	// 유저 GUID
	pUser->m_Guid = uuid;
	// 유저 디폴트 케릭터 GUID
	pUser->m_GuidActiveCharacter = uuidCharacter;

	{
		TSynchronize sync(this);
		TCharacter defaultChar;
		defaultChar.m_Info.tGuid = uuidCharacter;
		pUser->GetCharacterList().insert(make_pair(uuidCharacter, defaultChar));

		std::map<tGUID, TCharacter, GUIDComparer>::iterator iter;
		iter = pUser->GetCharacterList().find(uuidCharacter);
		if (iter != pUser->GetCharacterList().end())
		{
			pUser->SetActiveCharacter((TCharacter*)(&(iter->second)), true);
		}
		I_ServerIOCP.AddHandleToIOCP((HANDLE)sock, (ULONG_PTR)pUser);
		m_UserList.insert(make_pair(uuid, pUser));
		pUser->WaitForPacketReceive();
	}
	I_DebugStr.DisplayText("\nTServer::AddUser() %s[%d]\n", "User Count=", I_Server.m_UserList.size());
}

void TServer::ChangeSuperUser()
{
	TUserListItor superUser = m_UserList.begin();
	if (superUser == m_UserList.end())
		return;

	TUser* pUser = (TUser*)superUser->second;
	if (pUser)
	{
		pUser->m_iType = 1;
		TCharacterListItor iter = m_UserCharacterList.Get().find(pUser->m_GuidActiveCharacter);
		if (iter != m_UserCharacterList.Get().end())
		{
			iter->second.m_Info.iType = 1;
		}
	}
}


void TServer::PrintGuid(tGUID& tguid, const char* msg)
{
	string guidString;
	RPC_CSTR szUuid = NULL;
	if (::UuidToStringA(&tguid, &szUuid) == RPC_S_OK)
	{
		guidString = (char*)szUuid;
		::RpcStringFreeA(&szUuid);
	}
	I_DebugStr.DisplayText("\nTServer::PrintGuid() %s=%s\n", guidString.c_str(), msg);
}

ostream& operator<<(ostream& stream, TServer& info)
{
	{
		TSynchronize sync(&info);
		stream << info.m_UserList.size();
		for (auto& user : info.m_UserList)
		{
			stream << user.first;
			stream << (*user.second);
		}
	}
	return stream;
}
istream& operator>>(istream& stream, TServer& info)
{
	{
		TSynchronize sync(&info);
		size_t iNumUsers = 0;
		tGUID guid;
		TUser* pUser = NULL;
		SAFE_NEW(pUser, TUser);
		info.m_UserList.clear();

		stream >> iNumUsers;
		for (int i = 0; i < iNumUsers; i++)
		{
			stream >> guid;
			stream >> *pUser;
			info.m_UserList[guid] = pUser;
		}
	}
	return stream;
}

TServer::TServer()
{
}


TServer::~TServer()
{
}
