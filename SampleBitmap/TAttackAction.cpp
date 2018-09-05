#include "TAttackAction.h"

bool TAttackAction::Process(TObject* pTarget)
{
	return true;
}


TAttackAction::TAttackAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TAttackAction::~TAttackAction()
{
}
