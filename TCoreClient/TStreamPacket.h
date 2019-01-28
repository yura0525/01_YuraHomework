#pragma once
#include "TServerStd.h"
#include "TProtocol.h"
class TUser;
#define MAX_RECV_SIZE 1000

enum MSG_TYPE
{
	SEND_MSG_DAMAGE = 0,
	SEND_ATTACK,
};

class TStreamPacket
{
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos;
	int					m_iWritePos;
	int					m_iReadPos;
	bool				SendMsg(MSG_TYPE msg);
public:
	void Put(char* recvBuffer, int iRecvSize, TUser* = NULL);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

