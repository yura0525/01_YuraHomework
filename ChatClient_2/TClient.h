#pragma once
#include "TServerStd.h"
#include "TProtocolChat.h"
#include "TUser_1.h"

class TClient : public TUser_1
{
public:
	SOCKET			m_iSocket;
	HANDLE			m_hSendThread;
	HANDLE			m_hReceiveThread;
	bool			m_bExit;

public:
	char			m_strBuffer[128];
	bool			m_bSend;
	bool			m_bLoginOK;

public:
	bool			Init();
	bool			Frame();
	bool			Release();
	int				CreateConnectSocket(int iPort);
	int				SendMsg(char* pMsg, WORD code);
	int				SendMsg(char* pMsg, int iSize, WORD code);
	int				ProcessPacket();
public:
	TClient();
	virtual ~TClient();
};

