#include "stdafx.h"
#include "XUser.h"
#include "TLogError.h"
#include "TSynchronize.h"

void XUser::RecvData()
{
	//�ѹ��� 2048��ŭ ������ �޾Ҵ�.
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
void XUser::SendData()
{

}
void XUser::Dispatch(DWORD bytes, LPOVERLAPPED_EX ovex)
{
	{
		TSynchronize sync(this);
		if (ovex->m_iFlags == OVERLAPPED_EX::MODE_RECV)
		{
			//�޸𸮿� ���� �����Ͱ� �ִ�.
			//��Ŷó�� -> ��Ŷ �ϼ� -> ��Ŷ Ǯ�� ����.
			m_StreamPacket.Put(m_strBuffer, bytes, this);
			RecvData();
		}
		else
		{
			SendData();
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
