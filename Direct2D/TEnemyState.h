#pragma once
#include "xObject_2D.h"

class TEnemy;

class TEnemyState
{
protected:
	TEnemy * m_pOwner;
public:
	virtual bool Process(xObject_2D* pTarget) = 0;
public:
	TEnemyState(TEnemy* pOwner);
	virtual ~TEnemyState();
};

