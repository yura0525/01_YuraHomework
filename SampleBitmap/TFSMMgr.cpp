#include "TFSMMgr.h"

int TFSMMgr::Load(T_STR szLoadFIle)
{
	T_STR szName = szLoadFIle;

	map<int, TFiniteStateMachine*>::iterator iter;

	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		TFiniteStateMachine* pData = (TFiniteStateMachine*)(*iter).second;
		if (pData->m_szLoadFile == szLoadFIle)
		{
			return (*iter).first;
		}
	}
	TFiniteStateMachine* pData = new TFiniteStateMachine;
	if (pData->Load(szLoadFIle))
	{
		m_mapList.insert(make_pair(m_iIndex++, pData));
		return m_iIndex;
	}
	return -1;
}

TFiniteStateMachine* TFSMMgr::GetPtr(int iKey)
{
	std::map<int, TFiniteStateMachine*>::iterator iter;
	iter = m_mapList.find(iKey);
	if (iter == m_mapList.end())
		return NULL;

	return (TFiniteStateMachine*)(*iter).second;
}
bool TFSMMgr::Release()
{
	std::map<int, TFiniteStateMachine*>::iterator iter;

	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		TFiniteStateMachine* pData = (TFiniteStateMachine*)(*iter).second;
		delete pData;
	}

	m_mapList.clear();
	return true;
}

TFSMMgr::TFSMMgr()
{
	m_iIndex = 0;
}

TFSMMgr::~TFSMMgr()
{
}
