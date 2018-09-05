#include "TFiniteStateMachine.h"

void TFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutput)
{
	m_mapOutState[dwEvent] = dwOutput;
}

DWORD TFiniteState::GetState(DWORD dwEvent)
{
	std::map<DWORD, DWORD>::iterator iter;
	iter = m_mapOutState.find(dwEvent);

	if (iter != m_mapOutState.end())
	{
		return m_mapOutState[dwEvent];
	}

	return m_dwCurrentState;
}
TFiniteState::TFiniteState()
{

}
TFiniteState::~TFiniteState()
{

}

void TFiniteStateMachine::AddStateTransition(DWORD dwState, DWORD dwEvent, DWORD dwOutState)
{
	std::map<DWORD, TFiniteState*>::iterator iter;
	iter = m_mapState.find(dwState);

	TFiniteState* pState = NULL;

	if (iter != m_mapState.end())
	{
		pState = (*iter).second;
	}
	else
	{
		pState = new TFiniteState();
		m_mapState[dwState] = pState;
		pState->m_dwCurrentState = dwState;
	}
	pState->AddTransition(dwEvent, dwOutState);
}
DWORD TFiniteStateMachine::StateTransition(DWORD dwState, DWORD dwEvent)
{
	std::map<DWORD, TFiniteState*>::iterator iter;
	iter = m_mapState.find(dwState);

	if (iter != m_mapState.end())
	{
		return m_mapState[dwState]->GetState(dwEvent);
	}

	return dwState;
}

bool TFiniteStateMachine::Load(T_STR szLoadFile)
{
	m_szLoadFile = szLoadFile;

	FILE* fp;
	_wfopen_s(&fp, m_szLoadFile.c_str(), _T("rt"));
	if (fp == NULL) return false;

	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };
	int iNumCounter = 0;

	_fgetts(pBuffer, _countof(pBuffer), fp);
	_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumCounter);

	int iIndex = 0;
	TCHAR szInState[256] = { 0, };
	TCHAR szEventState[256] = { 0, };
	TCHAR szOutState[256] = { 0, };

	for (int iState = 0; iState < iNumCounter; iState++)
	{
		_fgetts(pBuffer, _countof(pBuffer), fp);
		_stscanf_s(pBuffer, _T("%d%s%s%s"), &iIndex,
			szInState, _countof(szInState),
			szEventState, _countof(szEventState),
			szOutState, _countof(szOutState));

		AddStateTransition(GetIndex(szInState), GetIndex(szEventState), GetIndex(szOutState));
	}
	fclose(fp);
	return true;
}
bool TFiniteStateMachine::Release()
{
	return true;
}
int TFiniteStateMachine::GetIndex(T_STR szState)
{
	if (_tcscmp(szState.c_str(), _T("STATE_STAND")) == 0)
	{
		return 0;
	}
	if (_tcscmp(szState.c_str(), _T("STATE_MOVE")) == 0)
	{
		return 1;
	}
	if (_tcscmp(szState.c_str(), _T("STATE_ATTACK")) == 0)
	{
		return 2;
	}

	//event
	if (_tcscmp(szState.c_str(), _T("EVENT_PATROL")) == 0)
	{
		return 0;
	}
	if (_tcscmp(szState.c_str(), _T("EVENT_FINDTARGET")) == 0)
	{
		return 1;
	}
	if (_tcscmp(szState.c_str(), _T("EVENT_LOSTTARGET")) == 0)
	{
		return 2;
	}
	if (_tcscmp(szState.c_str(), _T("EVENT_STOPWALK")) == 0)
	{
		return 3;
	}
	return -1;
}

TFiniteStateMachine::TFiniteStateMachine()
{
	//현상태 -> 이벤트 -> 출력 상태
	AddStateTransition(STATE_STAND, EVENT_PATROL, STATE_MOVE);
	AddStateTransition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
}
TFiniteStateMachine::~TFiniteStateMachine()
{

}

