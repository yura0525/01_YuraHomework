#pragma once
#include <Windows.h>
#include "TProtocol.h"

#define PACKETBUFFERSIZE 8192

class TPacket
{
public:
	typedef struct
	{
		WORD* len;
		WORD* type;
	}PACKETHEADER;
public:
	PACKETHEADER	m_PacketHeader;
	char			m_strPacketBuffer[PACKETBUFFERSIZE];
	char*			m_pstrWritePosition;
	char*			m_pstrReadPosition;

	WORD			GetID();
	WORD			GetSIze();
	int				m_iReceivedSize;
	int				GetDataSize();
	void			ReadData(void* buffer, int iSize);
	void			WriteData(void* buffer, int iSize);

	TPacket&		SetID(unsigned short ID);
	// 패킷을 생성할 때 
	TPacket&		operator<<(int arg);
public:
	TPacket();
	virtual ~TPacket();
};

