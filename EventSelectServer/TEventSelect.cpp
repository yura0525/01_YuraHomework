#include "TEventSelect.h"


bool TEventSelect::InitSelectServer()
{
	if (TNetwork::Init() == true)
	{
		SOCKADDR_IN server;
		ZeroMemory(&server, sizeof(SOCKADDR_IN));
		server.sin_family = AF_INET;
		server.sin_port = htons(10000);
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		int iRet = ::bind(m_Sock, (SOCKADDR*)&server, sizeof(server));
		if (iRet == SOCKET_ERROR)  false;
		iRet = listen(m_Sock, SOMAXCONN);
		if (iRet == SOCKET_ERROR)  false;

		m_EventArray[m_iCurrentEvent] = WSACreateEvent();
		WSAEventSelect(m_Sock, m_EventArray[m_iCurrentEvent], FD_ACCEPT | FD_CLOSE);
		m_UserList[m_iCurrentEvent].m_Socket = m_Sock;
		m_iCurrentEvent = 1;
	}
	return true;
}
bool TEventSelect::Run()
{
	return true;
}
void TEventSelect::RemoveSession(int iIndex)
{
	closesocket(m_UserList[iIndex].m_Socket);
	WSACloseEvent(m_EventArray[iIndex]);
	for (int icnt = iIndex; icnt < m_iCurrentEvent; icnt++)
	{
		m_EventArray[icnt] = m_EventArray[icnt + 1];
		m_UserList[icnt] = m_UserList[icnt + 1];
	}
	m_iCurrentEvent--;
}
void TEventSelect::SendProc(int iIndex)
{

}


TEventSelect::TEventSelect()
{
}


TEventSelect::~TEventSelect()
{
}
