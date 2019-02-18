#include "TSynchronize_1.h"

TSynchronize_1::TSynchronize_1(TServerObj_1* pSvrObj) : m_pServerObj(pSvrObj)
{
	if (m_pServerObj == NULL)
		return;
	EnterCriticalSection(&m_pServerObj->m_CS);
}



TSynchronize_1::~TSynchronize_1()
{
	if (m_pServerObj == NULL)
		return;
	LeaveCriticalSection(&m_pServerObj->m_CS);
}
