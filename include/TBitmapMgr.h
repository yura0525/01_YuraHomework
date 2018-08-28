#pragma once
#include "TBitmap.h"
using namespace std;

//template<class T>
//class TSingleton
//{
//public:
//	static T& GetInstance()
//	{
//		static T theInstance;
//		return theInstance;
//	}
//};
struct TCacheData
{
	int			iKey;
	T_STR		name;
	TBitmap*	m_pBitmap;
};

class TBitmapMgr : public TSingleton<TBitmapMgr>
{
public:
	friend class TSingleton<TBitmapMgr>;

	std::map<int, TBitmap*> m_mapList;
	std::list<TCacheData> m_mapCacheList;

	int m_iIndex;

public:
	int Load(T_STR szLoadFile);
	TBitmap* GetPtr(int iKey);

	void AddCache(int iKey, T_STR name, TBitmap* pData);
	int GetCache(T_STR szName);
	TBitmap* GetCache(int iKey);
protected:
	TBitmapMgr();
public:
	virtual ~TBitmapMgr();
};

#define I_BitmapMgr TBitmapMgr::GetInstance()
