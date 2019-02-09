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
	WSANETWORKEVENTS NetworkEvent;
	SOCKADDR_IN	clientaddr;
	SOCKET		client_sock;
	int			addrlen;

	// ��ü �̺�Ʈ�� �����Ѵ�.
	int iIndex = WSAWaitForMultipleEvents(m_iCurrentEvent, m_EventArray, 
		FALSE, // ��ü �̺�Ʈ�� ��޸� ���̳�? , �ƴϳ�?
		1000, FALSE);
	//WSA_INFINITE, 	FALSE );
	if (iIndex == WSA_WAIT_FAILED) return false;
	if (iIndex == WSA_WAIT_TIMEOUT)
	{
		return true;
	}

	iIndex -= WSA_WAIT_EVENT_0;
	// ��Ʈ��ũ �̺�Ʈ�� Ȯ���Ѵ�.
	// �߿� :  iIndex(�̺�Ʈ �迭�� �ε���)�� ���� �ش� ������ ã�Ƽ�
	//         � �̺�Ʈ���� Ȯ���Ѵ�.

	for (int iEvent = iIndex; iEvent < m_iCurrentEvent; iEvent++)
	{
		int iSignalIndex = WSAWaitForMultipleEvents(1, &m_EventArray[iEvent], TRUE, 0, FALSE);
		if (iSignalIndex == WSA_WAIT_FAILED || iSignalIndex == WSA_WAIT_TIMEOUT)
		{
			continue;
		}
		iSignalIndex = iEvent;

		int iRet = WSAEnumNetworkEvents(m_UserList[iSignalIndex].m_Socket, m_EventArray[iSignalIndex], &NetworkEvent);
		if (iRet == SOCKET_ERROR) return false;

		// � ��Ʈ��ũ �̺�Ʈ���� �� �� �ִ�.
		if (NetworkEvent.lNetworkEvents & FD_ACCEPT)
		{
			// ���� ���θ� �Ǵ��Ѵ�. 
			if (NetworkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				return false;
			}
			addrlen = sizeof(clientaddr);
			client_sock = accept(m_Sock, (SOCKADDR*)&clientaddr, &addrlen);

			// �ִ�
			if (m_iCurrentEvent < WSA_MAXIMUM_WAIT_EVENTS)
			{
				m_EventArray[m_iCurrentEvent] = WSACreateEvent();
				WSAEventSelect(client_sock, m_EventArray[m_iCurrentEvent], FD_READ | FD_WRITE | FD_CLOSE);
				m_UserList[m_iCurrentEvent].m_Socket = client_sock;
				m_iCurrentEvent++;
			}
		}
		//Recv
		if (NetworkEvent.lNetworkEvents & FD_READ)
		{
			// ���� ���θ� �Ǵ��Ѵ�. 
			if (NetworkEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				return false;
			}

			//recv
			char buf[1024] = { 0, };
			iRet = recv(m_UserList[iSignalIndex].m_Socket, buf, 1024, 0);

			if (iRet == SOCKET_ERROR || iRet == 0)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					continue;
				}
				RemoveSession(iSignalIndex);
			}
			else
			{
				m_UserList[iSignalIndex].m_RecvStreamPacket.Put(buf, iRet, &m_UserList[iSignalIndex]);
				SendProc(iSignalIndex);
			}
		}

		//Send
		if (NetworkEvent.lNetworkEvents & FD_WRITE)
		{
			// ���� ���θ� �Ǵ��Ѵ�. 
			if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				return false;
			}
			UPACKET packet;
			packet.ph.len = 4;
			packet.ph.type = PACKET_CHAT_MSG;
			int iRet = send(m_UserList[iIndex].m_Socket, (char*)&packet, packet.ph.len, 0);

			//send
			SendProc(iSignalIndex);
		}

		//close
		if (NetworkEvent.lNetworkEvents & FD_CLOSE)
		{
			// ���� ���θ� �Ǵ��Ѵ�. 
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				return false;
			}
			RemoveSession(iSignalIndex);
		}
	}

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
	//send
	for (std::list<UPACKET>::iterator itor = m_UserList[iIndex].m_SendPacketList.begin();
		itor != m_UserList[iIndex].m_SendPacketList.end(); itor++)
	{
		UPACKET packet = (UPACKET)*itor;
		int iRet = send(m_UserList[iIndex].m_Socket, (char*)&packet, packet.ph.len, 0);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				continue;
			}
		}
		else
		{
			if(m_UserList[iIndex].m_SendPacketList.size() <= 0)
				break;
		}
	}
}


TEventSelect::TEventSelect()
{
	m_iCurrentEvent = 0;
}


TEventSelect::~TEventSelect()
{
}
