#include "TCharacter.h"

ostream& operator<<(ostream& stream, TCharacter& tChar)
{
	{
		TSynchronize sync(&tChar);
		stream << tChar.m_Info;
	}
	return stream;
}
istream& operator>>(istream& stream, TCharacter& tChar)
{
	{
		TSynchronize sync(&tChar);
		stream >> tChar.m_Info;
	}
	return stream;
}

TCharacter::TCharacter()
{
	ZeroMemory(&m_Info, sizeof(TPACKET_CHARACTER_INFO));
	m_Info.fHealth = 1.0f;
	m_Info.bAlive = true;
	m_Info.bAttack = false;
}


TCharacter::~TCharacter()
{
}
