#include "TSynchronize.h"

TServerObj::TServerObj()
{
	InitializeCriticalSection(&m_cs);
}


TServerObj::~TServerObj()
{
	DeleteCriticalSection(&m_cs);
}


TSynchronize::TSynchronize(TServerObj* pObj) : m_pSerObj(pObj)
{
	if (pObj == NULL)
		return;
	EnterCriticalSection(&pObj->m_cs);
}


TSynchronize::~TSynchronize()
{
	if (m_pSerObj == NULL)
		return;
	LeaveCriticalSection(&m_pSerObj->m_cs);
}
