#include "TStandAction.h"

bool TStandAction::Process(TObject* pTarget)
{
	return true;
}

TStandAction::TStandAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TStandAction::~TStandAction()
{
}
