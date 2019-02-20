#pragma once
#include "TServerObj_1.h"
#include "TProtocol.h"

#define MAX_RECV_SIZE 10000

class TUser;
typedef struct
{
	UPACKET packet;
	TUser* pUser;
}T_PACKET, *P_TPACKET;

class TStreamPacket_3 : public TServerObj_1
{
public:
	std::list<T_PACKET>						m_PacketList;
	typedef std::list<T_PACKET>::iterator	m_PackItor;

public:
	UPACKET*		m_pPacket;
	char			m_RecvBuffer[MAX_RECV_SIZE];
	DWORD			m_iStartPos;
	DWORD			m_iWritePos;
	DWORD			m_iReadPos;

	void			Put(char* recvBuffer, int iRecvSize, TUser* pUser = NULL);

public:
	TStreamPacket_3();
	~TStreamPacket_3();
};

