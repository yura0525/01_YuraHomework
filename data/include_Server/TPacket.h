#pragma once
#include "TProtocol.h"

#define PACKET_BUFFER_SIZE 8192

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
	char			m_strPacketBuffer[PACKET_BUFFER_SIZE];
	char*			m_pstrWritePosition;
	char*			m_pstrReadPosition;
	int				m_iReceivedSize;

	WORD			GetID();
	WORD			GetSize();
	
	int				GetDataSize();
	void			ReadData(void* buffer, int iSize);
	void			WriteData(void* buffer, int iSize);

	TPacket&		SetID(unsigned short ID);
	// 패킷을 생성할 때 
	TPacket&		operator<<(int arg);
	// 패킷에서 데이터를 추출할 때
	TPacket&		operator>>(int& arg);

	TPacket&     operator << (DWORD arg);
	TPacket&     operator >> (DWORD& arg);

	TPacket&     operator << (bool arg);
	TPacket&     operator >> (bool& arg);

	TPacket&     operator << (unsigned short arg);
	TPacket&     operator >> (unsigned short& arg);

	TPacket&     operator << (float arg);
	TPacket&     operator >> (float& arg);

	TPacket&     operator << (char* arg);
	TPacket&     operator >> (char* arg);
	TPacket&     operator << (TPacket& arg);
	TPacket&     operator >> (TPacket& arg);

	char*		GetData();
	void		Clear();
public:
	TPacket();
	TPacket(unsigned short type);
	virtual ~TPacket();
};

