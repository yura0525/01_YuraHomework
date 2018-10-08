#pragma once
#include "TEnemy.h"

class TAttackAction : public TEnemyState
{
public:
	virtual bool Process(xObject_2D* pTarget);
public:
	TAttackAction(TEnemy * pOwner);
	virtual ~TAttackAction();
};

