#include "TMoveAction.h"

bool TMoveAction::Process(TObject* pTarget)
{
	return true;
}

TMoveAction::TMoveAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TMoveAction::~TMoveAction()
{
}
