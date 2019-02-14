#define _CRT_SECURE_NO_WARNINGS
#include "TClient.h"
#include "TDebugString.h"

unsigned __stdcall sendMessage(LPVOID arg)
{
	I_Debug.Print(const_cast<char*>("%s"), "send ������ ����");
	
	TClient* pClient = (TClient*)arg;

	while (!pClient->m_bExit)
	{
		if (pClient->m_bLoginOK && pClient->m_bSend)
		{
			if (pClient->SendMsg(pClient->m_strBuffer, PACKET_CHAT_MSG) <= 0)
			{
				break;
			}
			pClient->m_bSend = false;
		}
		if (SwitchToThread() == FALSE)
		{
			// Sleep(0)�� ������ �켱���� ������ ����Ī ������
			// Sleep(1)�� ���� �ٸ� �켱���� �����忡���� ����Ī�� �ȴ�.
			Sleep(1);
		}
	}
	return 0;
}

unsigned __stdcall receiveMessage(LPVOID arg)
{
	I_Debug.Print(const_cast<char*>("%s"), "receive ������ ����");
	TClient* pClient = (TClient*)arg;

	SOCKET iSocket = pClient->m_iSocket;
	char strBuffer[2048] = { 0, };

	while (!pClient->m_bExit)
	{
		int iRet = recv(iSocket, strBuffer, sizeof(strBuffer), 0);
		if (iRet <= 0)
		{
			//�񵿱� ����
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}

			pClient->m_bExit = true;
			closesocket(pClient->m_iSocket);
			break;
		}
		strBuffer[iRet] = 0;
		pClient->m_StreamPacket.Put(strBuffer, iRet, pClient);
		// Sleep(1) �Ǵ� SwitchToThread()�� ����ϸ�
		// �켱������ ���� �����忡�� ����Ī�� �����ϴ�.
		if (SwitchToThread() == FALSE)
		{
			Sleep(1);
		}
	}
	return 0;
}

bool TClient::Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}

	if (CreateConnectSocket(10000) == 0)
	{
		// send thread
		unsigned int sendThread, receiveThread;
		m_hSendThread = (HANDLE)_beginthreadex(NULL, 0, sendMessage, (VOID*)this, 0, &sendThread);

		//recv thread
		m_hReceiveThread = (HANDLE)_beginthreadex(NULL, 0, receiveMessage, (VOID*)this, 0, &receiveThread);

		int iLevelSend = GetThreadPriority(HANDLE(m_hSendThread));
		int iLevelRecv = GetThreadPriority(HANDLE(m_hReceiveThread));
		if (SetThreadPriority(HANDLE(m_hReceiveThread), THREAD_PRIORITY_ABOVE_NORMAL) == TRUE)
		{
			iLevelRecv = GetThreadPriority(HANDLE(m_hReceiveThread));
			INT kkkk = 0;
		}
		m_bLoginOK = TRUE;
	}

	return true;
}
bool TClient::Frame()
{
	if (m_bExit)
	{
		I_Debug.Print(const_cast<char*>("%s"), "������ ����Ǿ����ϴ�.");
		m_bExit = false;
	}
	return true;
}
bool TClient::Release()
{
	m_bExit = true;
	closesocket(m_iSocket);

	//send exit wait
	WaitForSingleObject(m_hSendThread, INFINITE);

	//recv exit wait
	WaitForSingleObject(m_hReceiveThread, INFINITE);

	WSACleanup();

	CloseHandle(m_hReceiveThread);
	CloseHandle(m_hSendThread);
	m_hReceiveThread = NULL;
	m_hSendThread = NULL;

	return true;
}
int	 TClient::CreateConnectSocket(int iPort)
{
	int     rv;
	m_iSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iSocket == INVALID_SOCKET)
	{
		return -1;
	}
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(iPort);
	rv = connect(m_iSocket, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rv == SOCKET_ERROR)
	{
		return -1;
	}

	//struct linger
	//{
	//	int l_onoff;   //linger �ɼ� ��뿩��
	//	int l_linger;  //linger time
	//}
	linger optval;
	int iLength = sizeof(optval);
	getsockopt(m_iSocket, SOL_SOCKET, SO_LINGER, (char*)&optval, &iLength);
	optval.l_onoff = 1;
	optval.l_linger = 1000;
	setsockopt(m_iSocket, SOL_SOCKET, SO_LINGER, (char*)&optval, iLength);


	int socketType1;
	int typeLen1 = sizeof(socketType1);
	getsockopt(m_iSocket, SOL_SOCKET,
		SO_TYPE,
		(char*)&socketType1, &typeLen1);

	if (socketType1 == SOCK_STREAM)
		I_Debug.Print(const_cast<char*>("%s"), "SOCK_STREAM.");
	else
		I_Debug.Print(const_cast<char*>("%s"), "SOCK_DGRAM");

	getsockopt(m_iSocket, SOL_SOCKET,
		SO_SNDBUF,
		(char*)&socketType1, &typeLen1);
	I_Debug.Print(const_cast<char*>("%s=%d"), "SO_SNDBUF", socketType1);

	getsockopt(m_iSocket, SOL_SOCKET,
		SO_RCVBUF,
		(char*)&socketType1, &typeLen1);
	I_Debug.Print(const_cast<char*>("%s=%d"), "SO_RCVBUF", socketType1);

	//////////////////////////////////////// ���� /////////////////////////////////
	//�񵿱� ������ ��쿡 �������� ��ƾ�� ���� �ݺ��� �� �ֱ� ������ 
	//�׽�Ʈ�� �ϱ� ���ؼ� �ټ��� Ŭ���̾�Ʈ ���α׷��� �ٿ�� ������ �ɰ��ϰ� ���ϵȴ�. 
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	//int iMode = 1;
	//ioctlsocket(m_iSocket, FIONBIO, (u_long FAR*) &iMode);	
	return 0;
}
int	 TClient::SendMsg(char* pMsg, WORD code)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sprintf(sendmsg.msg, pMsg);
	sendmsg.ph.type = code;
	sendmsg.ph.len = sizeof(PACKET_HEADER) + strlen(sendmsg.msg);
	int iRet = send(m_iSocket, (char*)&sendmsg, sendmsg.ph.len, 0);
	if (iRet <= 0)
	{
		m_bSend = false;
		//�񵿱� ����
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return 1;
		}
		return 0;
	}
	m_bSend = false;
	return 1;
}
int	 TClient::SendMsg(char* pMsg, int iSize, WORD code)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	//sprintf(sendmsg.msg, pMsg);
	memcpy(sendmsg.msg, pMsg, iSize);
	sendmsg.ph.type = code;
	sendmsg.ph.len = sizeof(PACKET_HEADER) + iSize;
	int iRet = send(m_iSocket, (char*)&sendmsg, sendmsg.ph.len, 0);
	if (iRet <= 0)
	{
		m_bSend = false;
		//�񵿱� ����
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return 1;
		}
		return 0;
	}
	m_bSend = false;
	return 1;
}

// �ϼ��� ��Ŷ�� ���� ó��
int	 TClient::ProcessPacket()
{
	{
		TSynchronize sync(this);
		std::list<T_PACKET>::iterator iter;
		for (iter = m_StreamPacket.m_PacketList.begin(); iter != m_StreamPacket.m_PacketList.end(); iter++)
		{
			UPACKET* pPacket = &(*iter).packet;
			switch (pPacket->ph.type)
			{
			case PACKET_USER_POSITION:
			{
				TPACKET_USER_POSITION user;
				memcpy(&user, pPacket->msg, sizeof(char) * pPacket->ph.len - 4);
				if (user.direction == VK_DOWN)
				{
					// user.x += 10 *  g_fSecPerfame;
					int kkkk = 0;
				}
				if (user.direction == VK_LEFT)
				{
					// user.x += 10 *  g_fSecPerfame;
					int kkkk = 0;
				}
				if (user.direction == VK_RIGHT)
				{
					// user.x += 10 *  g_fSecPerfame;
					int kkkk = 0;
				}
				if (user.direction == VK_UP)
				{
					// user.x += 10 *  g_fSecPerfame;
					int kkkk = 0;
				}
			}
			break;
			case PACKET_CHAT_NAME_REQ:
			{
				I_Debug.Print(const_cast<char*>("%s"), pPacket->msg);

			}
			break;
			case PACKET_CHAT_MSG:
			{
				I_Debug.Print(const_cast<char*>("%s"), pPacket->msg);
			}
			break;
			}
		}
		m_StreamPacket.m_PacketList.clear();
	}
	return 1;
}
TClient::TClient()
{
	m_bSend = false;
	m_bLoginOK = false;
	m_bExit = false;
	m_iSocket = 0;
	m_hReceiveThread = 0;
	m_hSendThread = 0;
}


TClient::~TClient()
{
}
