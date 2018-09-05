#include "TPCObject.h"


void TPCObject::SetHP(int hp)
{
	m_iHP = hp;
}

void TPCObject::ProcessDamage(int damage)
{
	m_iHP = m_iHP + damage;
	if (m_iHP <= 0)
		m_bDead = true;
}

TPCObject::TPCObject()
{
}


TPCObject::~TPCObject()
{
}
