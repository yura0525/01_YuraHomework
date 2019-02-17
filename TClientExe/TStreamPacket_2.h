#pragma once
#include "TServerStd.h"
#include <process.h>
#include "TProtocol.h"

class TStreamPacket_2
{
public:
	std::list<UPACKET>						m_PacketList;
	typedef std::list<UPACKET>::iterator	m_PackItor;
public:
	char			m_strBuffer[4096];// Recv����+ 1���� ��Ŷ ũ��
	int				m_iWritePos;
	P_UPACKET		m_pPacketData;
	int				m_iRecvPos;

	UPACKET			m_PacketData;
	int				m_iDataLength;
	PACKET_HEADER	m_Header;
public:

	void			Put(char* recvBuffer, int iRecvSize);

public:
	TStreamPacket_2();
	~TStreamPacket_2();
};

