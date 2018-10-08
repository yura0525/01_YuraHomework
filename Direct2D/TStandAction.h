#pragma once
#include "TEnemy.h"
class TStandAction : public TEnemyState
{
public:
	virtual bool Process(xObject* pTarget) override;
public:
	TStandAction(TEnemy * pOwner);
	virtual ~TStandAction();
};

