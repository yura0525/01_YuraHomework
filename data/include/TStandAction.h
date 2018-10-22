#pragma once
#include "TEnemy.h"

class TStandAction : public TEnemyState
{
public:
	virtual bool Process(TObject* pTarget);

public:
	TStandAction(TEnemy * pOwner);
	virtual ~TStandAction();
};

