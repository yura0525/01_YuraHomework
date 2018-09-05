#include "TFiniteStateMachine.h"

void TFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutput)
{

}

DWORD TFiniteState::GetState(DWORD dwEvent)
{
	return m_dwCurrentState;
}
TFiniteState::TFiniteState()
{

}
TFiniteState::~TFiniteState()
{

}

TFiniteStateMachine::TFiniteStateMachine()
{
}


TFiniteStateMachine::~TFiniteStateMachine()
{
}
