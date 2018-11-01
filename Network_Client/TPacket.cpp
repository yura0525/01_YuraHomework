#include "TPacket.h"

void TPacket::PutData(const char* pData, int iSize)
{
	m_uPacket.ph.len += iSize;
	memcpy(m_pOffset, pData, iSize);
	m_pOffset += iSize;
}

TPacket& TPacket::operator<<(const char* pData)
{
	int iSize = strlen(pData) + 1;
	PutData(pData, iSize);
	return *this;
}
TPacket& TPacket::operator<<(int data)
{
	PutData((char*)&data, sizeof(data));
	return *this;
}
TPacket& TPacket::operator<<(string data)
{
	PutData((char*)data.c_str(), data.size());
	return *this;
}

void TPacket::GetData(const char* pData, int iSize)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iSize);
	m_pOffset += iSize;
}
TPacket& TPacket::operator>>(const char* pData)
{
	//스트링의 끝을 알수있게 널문자를 넣어야한다.
	//iSize는 스트링의 끝까지 해서 문자열의 길이를 얻는다.
	int iSize = strlen(m_pOffset) + 1;
	GetData(pData, iSize);
	return *this;
}
TPacket& TPacket::operator>>(int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
TPacket& TPacket::operator>>(string& data)
{
	//스트링의 끝을 알수있게 널문자를 넣어야한다.
	//iSize는 스트링의 끝까지 해서 문자열의 길이를 얻는다.
	int iSize = strlen(m_pOffset) + 1;
	char buffer[256] = { 0, };
	GetData(buffer, iSize);
	data = buffer;

	return *this;
}
TPacket::TPacket(WORD type)
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.type = type;
	m_pOffset = m_uPacket.msg;
}

TPacket::TPacket()
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_pOffset = m_uPacket.msg;
}

TPacket::~TPacket()
{
}
