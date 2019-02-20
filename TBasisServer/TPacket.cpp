#include "TPacket.h"

// TPacket packet;
// int id, ps, money;
// char email[30], address[30];
// packet.SetID(1002) << id << ps;
// packet.SetID(1002) << id << ps << email << addree << money;
// packet.SetID(1002) >> id >> ps >> email >> addree >> money;

// TPacket packet(1002);
// packet << id << ps;
// packet >> id >> ps;

WORD TPacket::GetID()
{
	return *(m_PacketHeader.type);
}
WORD TPacket::GetSize()
{
	return *(m_PacketHeader.len);
}
int TPacket::GetDataSize()
{
	return m_iReceivedSize;
}
void TPacket::ReadData(void* buffer, int iSize)
{
	::CopyMemory(buffer, m_pstrReadPosition, iSize);
	m_pstrReadPosition += iSize;
}
void TPacket::WriteData(void* buffer, int iSize)
{
	if (m_iReceivedSize >= PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE)
	{
		// 오류
	}
	*(m_PacketHeader.len) += iSize;
	CopyMemory(m_pstrWritePosition, buffer, iSize);
	// 1차원 배열의 주소가 이동
	m_pstrWritePosition += iSize;
	// 실제 데이터의 바이트 용량
	m_iReceivedSize += iSize;
}
TPacket& TPacket::SetID(unsigned short ID)
{
	*(m_PacketHeader.type) = ID;
	return *this;
}

// 패킷을 생성할 때 sum = a.v + b.v + c.v + d.v + e.v;
TPacket& TPacket::operator<<(int arg)
{
	WriteData(&arg, sizeof(int));
	return *this;
}
// 패킷에서 데이터를 추출할 때
TPacket& TPacket::operator>>(int& arg)
{
	ReadData(&arg, sizeof(int));
	return *this;
}

TPacket& TPacket::operator << (DWORD arg)
{
	WriteData(&arg, sizeof(DWORD));
	return *this;
}
TPacket& TPacket::operator >> (DWORD& arg)
{
	ReadData(&arg, sizeof(DWORD));
	return *this;
}
TPacket& TPacket::operator << (bool arg)
{
	WriteData(&arg, sizeof(bool));
	return *this;
}
TPacket& TPacket::operator >> (bool& arg)
{
	ReadData(&arg, sizeof(bool));
	return *this;
}
TPacket& TPacket::operator << (unsigned short arg)
{
	WriteData(&arg, sizeof(unsigned short));
	return *this;
}
TPacket& TPacket::operator >> (unsigned short& arg)
{
	ReadData(&arg, sizeof(unsigned short));
	return *this;
}
TPacket& TPacket::operator << (float arg)
{
	WriteData(&arg, sizeof(float));
	return *this;
}
TPacket& TPacket::operator >> (float& arg)
{
	ReadData(&arg, sizeof(float));
	return *this;
}
TPacket& TPacket::operator << (char* arg)
{
	int iLen = sizeof(char) * strlen(arg);
	WriteData(arg, iLen);
	return *this;
}
TPacket& TPacket::operator >> (char* arg)
{
	int iLen = sizeof(char) * strlen(this->m_pstrReadPosition);
	ReadData(arg, sizeof(char) * iLen);
	return *this;
}
TPacket& TPacket::operator << (TPacket& arg)
{
	unsigned short type = arg.GetID();
	unsigned short size = arg.GetSize();

	WriteData(&type, sizeof(unsigned short));
	WriteData(&size, sizeof(unsigned short));
	WriteData(arg.m_pstrWritePosition, size - PACKET_HEADER_SIZE);
	return *this;
}
TPacket& TPacket::operator >> (TPacket& arg)
{
	int type, size;
	char buffer[4096] = { 0, };

	ReadData(&type, sizeof(unsigned short));
	ReadData(&size, sizeof(unsigned short));
	ReadData(buffer, size - PACKET_HEADER_SIZE);

	arg.SetID(type);
	arg.WriteData(buffer, size - PACKET_HEADER_SIZE);
	return *this;
}

char* TPacket::GetData()
{
	return m_strPacketBuffer;
}
void TPacket::Clear()
{
	ZeroMemory(m_strPacketBuffer, PACKET_BUFFER_SIZE);
	m_PacketHeader.len = (WORD*)m_strPacketBuffer + 0;
	m_PacketHeader.type = (WORD*)m_strPacketBuffer + 2;
	m_pstrWritePosition = &m_strPacketBuffer[4];
	m_pstrReadPosition = &m_strPacketBuffer[4];
	m_iReceivedSize = 0;
	*(m_PacketHeader.len) = 4;
}

TPacket::TPacket()
{
	Clear();
	SetID(0);
}
TPacket::TPacket(unsigned short type)
{
	Clear();
	SetID(type);
}

TPacket::~TPacket()
{
}
