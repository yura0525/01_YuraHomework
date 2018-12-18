#include "stdafx.h"
#include "XUser.h"
#include "TLogError.h"
#include "TSynchronize.h"

void XUser::RecvData()
{
	//한번에 2048만큼 통으로 받았다.
	m_wsaBuffer.buf = m_strBuffer;
	m_wsaBuffer.len = 2048;
	m_ov.m_iFlags = OVERLAPPED_EX::MODE_RECV;

	DWORD transfer, flags = 0;
	int iRet = WSARecv(m_clientSocket, &m_wsaBuffer, 1, &transfer, &flags, (LPOVERLAPPED)&m_ov, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (iRet != WSA_IO_PENDING)
		{
			I_LOG.T_ERROR();
		}
	}
}

int XUser::SendData(UPACKET* pPacket)
{
	DWORD sendbytes = 0, flags = 0;

	int iTotalsize = pPacket->ph.len + PACKET_HEADER_SIZE;
	pPacket->ph.type = PACKET_CHAT_MSG;

	memcpy(m_strBuffer, (char*)pPacket, sizeof(char) * iTotalsize);

	m_wsaBuffer.buf = m_strBuffer;
	m_wsaBuffer.len = 2048;
	m_ov.m_iFlags = OVERLAPPED_EX::MODE_SEND;
	
	int iSend = 0;
	do
	{
		iSend = WSASend(m_clientSocket, &m_wsaBuffer, 1, &sendbytes, flags, (LPOVERLAPPED)&m_ov, NULL);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return sendbytes;
}

int XUser::CheckReturn(int iRet)
{
	if (iRet == 0)
	{
		printf("\n---------->정상 퇴장");
		return 0;
	}

	if (iRet <= SOCKET_ERROR)
	{
		printf("\n----------->비정상 퇴장");
		I_LOG.T_ERROR();
		return -1;
	}
	return iRet;
}
void XUser::Dispatch(DWORD bytes, LPOVERLAPPED_EX ovex)
{
	{
		TSynchronize sync(this);
		if (ovex->m_iFlags == OVERLAPPED_EX::MODE_RECV)
		{
			//메모리에 받은 데이터가 있다.
			//패킷처리 -> 패킷 완성 -> 패킷 풀에 저장.
			m_StreamPacket.Put(m_strBuffer, bytes, this);
			RecvData();
		}
		else
		{
			//SendData();
		}
	}
}

XUser::XUser()
{
}

XUser::XUser(SOCKET _Sock, SOCKADDR_IN _Addr)
{
	m_clientSocket = _Sock;
	m_clientAddr = _Addr;
}

XUser::XUser(const XUser& user)
{
	m_clientSocket = user.m_clientSocket;
	m_clientAddr = user.m_clientAddr;
}

XUser::~XUser()
{
}


void XUserManager::Run()
{
	{
		TSynchronize(this);
		list<T_PACKET>::iterator itor;
		for (itor = m_PacketList.m_List.begin(); itor != m_PacketList.m_List.end(); itor++)
		{
			LP_TPACKET pData = &(*itor);
			UPACKET* pPacket = &(pData->packet);
			switch (pPacket->ph.type)
			{
			case PACKET_CHAT_MSG:
				//브로드캐스트 해야함.
				Broadcastting(pPacket);
				break;
			
			}
		}
		Sleep(1);
	}
}
void XUserManager::AddPacket(T_PACKET& packet)
{
	{
		TSynchronize(this);
		this->m_PacketList.m_List.push_back(packet);
	}
}

XUser* XUserManager::GetMemory(SOCKET clientSocket, SOCKADDR_IN clientAddr)
{
	XUser* pUser = new XUser(clientSocket, clientAddr);
	return pUser;
}
XUser* XUserManager::AddUser(SOCKET clientSocket, SOCKADDR_IN clientAddr)
{
	XUser* pUser = NULL;
	{
		TSynchronize sync(this);
		pUser = GetMemory(clientSocket, clientAddr);
		m_UserList.push_back(pUser);
	}
	return pUser;
}

bool XUserManager::DelUser(SOCKET clientSocket)
{
	{
		TSynchronize sync(this);
		std::list<XUser*>::iterator itor;
		std::list<XUser*>::iterator delItor = m_UserList.end();
		for (itor = m_UserList.begin(); itor != m_UserList.end(); itor++)
		{
			XUser* pUser = (XUser*)(*itor);
			if (pUser->m_clientSocket == clientSocket)
			{
				delItor = itor;
				break;
			}
		}

		if (delItor != m_UserList.end())
		{
			closesocket(clientSocket);
			m_UserList.erase(delItor);
		}
	}
	return true;
}

int XUserManager::Broadcastting(UPACKET* pPacket)
{
	{
		TSynchronize sync(this);
		std::list<XUser*>::iterator itor;
		for (itor = m_UserList.begin(); itor != m_UserList.end(); itor++)
		{
			XUser* pUser = (XUser*)(*itor);
			pUser->SendData(pPacket);
		}
	}
	return true;
}