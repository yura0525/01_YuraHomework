#pragma once
#include "TServerObj.h"
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TAcceptor.h"
#include "TDebugString.h"
#include "TTargetPointMgr.h"

class TCharacter : public TServerObj
{
public:
	TPACKET_CHARACTER_INFO m_Info;
public:
	friend ostream& operator<<(ostream& stream, TCharacter& tChar);
	friend istream& operator>>(istream& stream, TCharacter& tChar);
public:
	TCharacter();
	virtual ~TCharacter();
};

