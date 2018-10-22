#pragma once
#include "TEnemy.h"

class TAttackAction : public TEnemyState
{
public:
	virtual bool Process(TObject* pTarget);

public:
	TAttackAction(TEnemy * pOwner);
	virtual ~TAttackAction();
};

