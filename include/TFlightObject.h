#pragma once
#include "TObject.h"

class TFlightObject : public TObject
{
public:
	int	m_iHP;

public:
	void SetHP(int hp);
	void ProcessDamage(int damage);

public:
	TFlightObject();
	virtual ~TFlightObject();
};

