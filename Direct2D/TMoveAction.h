#pragma once
#include "TEnemy.h"

class TMoveAction : public TEnemyState
{
public:
	virtual bool Process(xObject* pTarget) override;
public:
	TMoveAction(TEnemy * pOwner);
	virtual ~TMoveAction();
};

