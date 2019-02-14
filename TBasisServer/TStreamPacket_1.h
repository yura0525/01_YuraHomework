#pragma once
#include "TServerStd.h"
#include "TProtocol.h"

#define MAX_RECV_SIZE 10000
class TUser;

typedef struct {
	UPACKET packet;
	TUser*	pUSer;
}T_PACKET, *P_TPACKET;

class TStreamPacket_1
{
public:
	std::list<T_PACKET>						m_PacketList;
	typedef std::list<T_PACKET>::iterator	m_PackItor;

public:
	UPACKET * m_pPacket;
	char m_RecvBuffer[MAX_RECV_SIZE];
	int m_iStartPos;
	int m_iWritePos;
	int m_iReadPos;
public:
	void Put(char* recvBuffer, int iSize, TUser* = NULL);

public:
	TStreamPacket_1();
	virtual ~TStreamPacket_1();
};

