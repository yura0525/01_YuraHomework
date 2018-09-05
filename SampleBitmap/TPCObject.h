#pragma once
#include "TObject.h"

class TPCObject : public TObject
{
public:
	int	m_iHP;

public:
	void SetHP(int hp);
	void ProcessDamage(int damage);

public:
	TPCObject();
	virtual ~TPCObject();
};

