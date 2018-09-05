#pragma once
#include "TEnemyState.h"

class TMoveAction : public TEnemyState
{
public:
	virtual bool Process(TObject* pTarget);
public:
	TMoveAction(TEnemy * pOwner);
	virtual ~TMoveAction();
};

