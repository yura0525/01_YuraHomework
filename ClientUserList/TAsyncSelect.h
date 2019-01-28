#pragma once
#include "TNetwork.h"

class TAsyncSelect : public TNetwork
{
public:
	char			m_szData[2048];
	TStreamPacket	m_StreamPacket;
public:
	int			RecvMsg();
	int			SendMsg();
	int			SendMsg(char* msg, WORD type);
	int			SendMsg(SOCKET sock, char* msg, WORD type);
	void		MsgEvent(MSG msg) {};
	bool		Connect(char* ip, int iPort);

	TAsyncSelect();
	virtual ~TAsyncSelect();
};

