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
	//��Ʈ���� ���� �˼��ְ� �ι��ڸ� �־���Ѵ�.
	//iSize�� ��Ʈ���� ������ �ؼ� ���ڿ��� ���̸� ��´�.
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
	//��Ʈ���� ���� �˼��ְ� �ι��ڸ� �־���Ѵ�.
	//iSize�� ��Ʈ���� ������ �ؼ� ���ڿ��� ���̸� ��´�.
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
