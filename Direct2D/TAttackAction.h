#pragma once
#include "TEnemy.h"

class TAttackAction : public TEnemyState
{
public:
	virtual bool Process(xObject* pTarget) override;
public:
	TAttackAction(TEnemy * pOwner);
	virtual ~TAttackAction();
};

