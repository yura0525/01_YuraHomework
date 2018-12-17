#include "stdafx.h"
#include "TSynchronize.h"


TSynchronize::TSynchronize(TSvrObject* pObj)
{
	if (pObj == NULL)
		return;
	m_pSerObj = pObj;
	EnterCriticalSection(&m_pSerObj->m_cs);
}


TSynchronize::~TSynchronize()
{
	if (m_pSerObj == NULL)
		return;
	LeaveCriticalSection(&m_pSerObj->m_cs);
}
