#pragma once
#include "TEnemy.h"
class TStandAction : public TEnemyState
{
public:
	virtual bool Process(xObject_2D* pTarget);
public:
	TStandAction(TEnemy * pOwner);
	virtual ~TStandAction();
};

