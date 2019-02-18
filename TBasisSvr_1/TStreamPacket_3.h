#pragma once
#include "TServerObj_1.h"
#include <process.h>
#include "TProtocol.h"

struct TUser;
typedef struct
{
	UPACKET packet;
	TUser* pUser;
}T_PACKET, *T_UPACKET;

class TStreamPacket_3
{
public:
	std::list<T_PACKET>						m_PacketList;
	typedef std::list<T_PACKET>::iterator	m_PackItor;

public:
	char			m_strBuffer[4096];// Recv����+ 1���� ��Ŷ ũ��
	int				m_iWritePos;
	P_UPACKET		m_pPacketData;
	int				m_iRecvPos;

	UPACKET			m_PacketData;
	int				m_iDataLength;
	PACKET_HEADER	m_Header;
	void			Put(char* recvBuffer, int iRecvSize, TUser* pUser);

public:
	TStreamPacket_3();
	~TStreamPacket_3();
};

