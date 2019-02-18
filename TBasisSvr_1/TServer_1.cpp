#define _CRT_SECURE_NO_WARNINGS
#include "TServer_1.h"
#include "TDebugString_2.h"
#include "TServerIOCP_1.h"

bool TServer_1::Init()
{
	int iRet;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

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
	std::list<TUser*>::iterator iter;
	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)*iter;
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

		T_PACKET* pSendUser = &(*iter);
		UPACKET* pPacket = &(pSendUser->packet);

		switch (pPacket->ph.type)
		{
		case PACKET_CHAT_NAME_ACK:
		{
			WaitForSingleObject(m_Mutex, INFINITE);
			PacketMsg.ph.len = pPacket->ph.len;
			PacketMsg.ph.type = PACKET_CHAT_MSG;

			pPacket->msg[pPacket->ph.len - 4] = 0;
			C_STR name = pPacket->msg;
			memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len - 4);

			char buffer[64] = "님이 입장하였습니다.";
			strcpy(&PacketMsg.msg[pPacket->ph.len - 4], buffer);
			PacketMsg.ph.len += strlen(buffer);

			I_DebugStr.DisplayText(const_cast<char*>("Message:%s\r\n"), PacketMsg.msg);

			std::list<TUser*>::iterator iter;

			for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
			{
				TUser* pUser = (TUser*)*iter;
				if (pUser->m_Socket != pSendUser->pUser->m_Socket)
				{
					retval = send(pUser->m_Socket, (char*)&PacketMsg, PacketMsg.ph.len, 0);
					if (retval == SOCKET_ERROR)
					{
						break;
					}
				}
				else
				{
					pUser->m_Name = name;
				}
			}
		}
		break;
		case PACKET_CHAT_MSG:
		{
			I_DebugStr.DisplayText(const_cast<char*>("Message:%s\r\n"), pPacket->msg);
			std::list<TUser*>::iterator	iter;

			for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
			{
				TUser* pUser = (TUser*)*iter;
				if (pUser->m_Socket != pSendUser->pUser->m_Socket)
				{
					retval = send(pUser->m_Socket, (char*)&pPacket, pPacket->ph.len, 0);
					if (retval == SOCKET_ERROR)
					{
						break;
					}
				}
			}

		}break;
		case PACKET_USER_POSITION:
		{
			I_DebugStr.DisplayText(const_cast<char*>("Message:%s\r\n"), pPacket->msg);
			std::list<TUser*>::iterator	iter;
			for (iter = I_Server.m_UserList.begin(); iter != I_Server.m_UserList.end(); iter++)
			{
				TUser* pUser = (TUser*)*iter;
				retval = send(pUser->m_Socket, (char*)&pPacket, pPacket->ph.len, 0);
				if (retval == SOCKET_ERROR)
				{
					break;
				}
			}
		}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	ReleaseMutex(m_Mutex);
	return;
}
TUser* TServer_1::GetUser(int iIndex)
{
	WaitForSingleObject(m_Mutex, INFINITE);
	std::list<TUser*>::iterator iter;

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)*iter;
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
	std::list<TUser*>::iterator iter;
	std::list<TUser*>::iterator delUser;

	for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		TUser* pUser = (TUser*)*iter;
		if (pUser->m_iEvent == iIndex)
		{
			I_DebugStr.DisplayText(const_cast<char*>("%s%s\r\n"), pUser->m_Name.c_str(), "님이 나가셨습니다.");
			closesocket(pUser->m_Socket);
			delUser = iter;
			break;
		}
	}
	m_UserList.erase(iter);
	m_iClientNumber--;
	ReleaseMutex(m_Mutex);
	return true;
}

TServer_1::TServer_1()
{
	m_iClientNumber = 0;
}


TServer_1::~TServer_1()
{
}
