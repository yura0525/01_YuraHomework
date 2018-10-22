#pragma once
#include "TFiniteStateMachine.h"
#include <map>
using namespace std;

class TFSMMgr : public TSingleton<TFSMMgr>
{
public:
	friend class TSingleton<TFSMMgr>;

	std::map<int, TFiniteStateMachine*> m_mapList;
	int		m_iIndex;
	
public:
	int Size()
	{
		return m_mapList.size();
	}
public:
	int Load(T_STR szLoadFile);
	TFiniteStateMachine* GetPtr(int iKey);
	bool Release();

protected:
	TFSMMgr();
public:
	virtual ~TFSMMgr();
};

#define I_FSMMgr TFSMMgr::GetInstance()
