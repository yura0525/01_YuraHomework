#include "TUser_1.h"

bool TUser_1::Create()
{
	ZeroMemory(&m_ov, sizeof(m_ov));
	WaitForPacketReceive();
	return true;
}
bool TUser_1::Run()
{
	char szMsgBuffer[2048] = { 0, };
	int strlen = 0;
	while (1)
	{
		strlen = recv(m_Socket, szMsgBuffer, sizeof(szMsgBuffer), 0);
		if (strlen <= 0)
			break;
		szMsgBuffer[strlen] = 0;
		m_StreamPacket.Put(szMsgBuffer, strlen, this);
		Sleep(10);
	}

	DeleteUser(m_Socket);
	return true;
}
void TUser_1::AddPacket(T_PACKET& addPacket)
{
	{
		TSynchronize sync(this);
		m_StreamPacket.m_PacketList.push_back(addPacket);
	}
	return;
}

void TUser_1::DeleteUser()
{

}
void TUser_1::DeleteUser(SOCKET socket)
{

}
void TUser_1::WaitForPacketReceive()
{
	DWORD cbTransferred, flags = 0;
	m_szRecvBuffer.buf = m_strBuffer;
	m_szRecvBuffer.len = 4096;

	int iRet = WSARecv(m_Socket, &m_szRecvBuffer, 1, &cbTransferred, 
						&flags, (LPOVERLAPPED)&m_ov, NULL);
	if (iRet == SOCKET_ERROR)
	{
		int dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING)
		{
			int kkk = 0;
		}
	}
}

void TUser_1::Dispatch(DWORD dwByteSize, LPOVERLAPPED ov)
{
	OVERLAPPEDEX* pOV = (OVERLAPPEDEX*)ov;
	if (dwByteSize == 0)
		return;
	if (pOV->iFlags == OVERLAPPEDEX::MODE_RECV)
	{
		m_StreamPacket.Put(m_szRecvBuffer.buf, dwByteSize, this);
	}
	if (pOV->iFlags == OVERLAPPEDEX::MODE_SEND)
	{

	}
	WaitForPacketReceive();
}

TUser_1::TUser_1()
{
	m_Socket = 0;
}


TUser_1::~TUser_1()
{
	closesocket(m_Socket);
}
