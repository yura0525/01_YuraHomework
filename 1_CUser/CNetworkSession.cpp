#include "CNetworkSession.h"
#include "CThreadSync.h"


CNetworkSession::CNetworkSession()
{
	ZeroMemory(&m_AcceptOverlapped, sizeof(m_AcceptOverlapped));
	ZeroMemory(&m_ReadOverlapped, sizeof(m_ReadOverlapped));
	ZeroMemory(&m_WriteOverlapped, sizeof(m_WriteOverlapped));
	ZeroMemory(m_ReadBuffer, sizeof(m_ReadBuffer));
	m_Socket = NULL;

	ZeroMemory(&m_UDPRemoteInfo, sizeof(m_UDPRemoteInfo));
	m_ReliableUDPThreadHandle = NULL;
	m_ReliableUDPThreadStartupEvent = NULL;
	m_ReliableUDPThreadDestroyEvent = NULL;
	m_ReliableUDPThreadWakeUpEvent = NULL;
	m_ReliableUDPWriteCompleteEvent = NULL;
	m_IsReliableUDPSending = FALSE;

	m_AcceptOverlapped.IoType = IO_ACCEPT;
	m_ReadOverlapped.IoType = IO_READ;
	m_WriteOverlapped.IoType = IO_WRITE;

	m_AcceptOverlapped.Object = this;
	m_ReadOverlapped.Object = this;
	m_WriteOverlapped.Object = this;
}


CNetworkSession::~CNetworkSession()
{
}

//개체 시작
BOOL CNetworkSession::Begin(VOID)
{
	CMultiThreadSync<int> Sync;		//다중스레드 동기화
	return TRUE;
}
BOOL CNetworkSession::End(VOID)
{
	return TRUE;
}
