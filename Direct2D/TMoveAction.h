#pragma once
#include "TEnemy.h"

class TMoveAction : public TEnemyState
{
public:
	virtual bool Process(xObject_2D* pTarget);
public:
	TMoveAction(TEnemy * pOwner);
	virtual ~TMoveAction();
};

