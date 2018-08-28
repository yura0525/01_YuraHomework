#include "TFlightObject.h"


void TFlightObject::SetHP(int hp)
{
	m_iHP = hp;
}

void TFlightObject::ProcessDamage(int damage)
{
	m_iHP = m_iHP + damage;
	if (m_iHP <= 0)
		m_bDead = true;
}

TFlightObject::TFlightObject()
{
}


TFlightObject::~TFlightObject()
{
}
