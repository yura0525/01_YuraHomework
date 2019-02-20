#include "TSynchronize_2.h"



TSynchronize_2::TSynchronize_2(TServerObj_2 * pSvrObj) : m_pServerObj(pSvrObj)
{
	if (m_pServerObj == NULL)
		return;

	EnterCriticalSection(&m_pServerObj->m_CS);
}


TSynchronize_2::~TSynchronize_2()
{
	if (m_pServerObj == NULL)
		return;
	LeaveCriticalSection(&m_pServerObj->m_CS);
}
