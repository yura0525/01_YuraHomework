#pragma once
#include <iostream>
#include "TProtocol.h"
#include <string.h>
using namespace std;

class TPacket
{
public:
	UPACKET		m_uPacket;
	char*		m_pOffset;
public:
	void PutData(const char* pData, int iSize);
	TPacket& operator<<(const char* pData);
	TPacket& operator<<(int data);
	TPacket& operator<<(string data);
public:
	void GetData(const char* pData, int iSize);
	TPacket& operator>>(const char* pData);
	TPacket& operator>>(int& data);
	TPacket& operator>>(string& data);
public:

	TPacket();
	TPacket(WORD type);
	virtual ~TPacket();
};

