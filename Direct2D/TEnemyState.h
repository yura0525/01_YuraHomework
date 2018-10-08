#pragma once
#include "xObject.h"

class TEnemy;

class TEnemyState
{
protected:
	TEnemy * m_pOwner;
public:
	virtual bool Process(xObject* pTarget) = 0;
public:
	TEnemyState(TEnemy* pOwner);
	virtual ~TEnemyState();
};

