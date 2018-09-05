#include "TBitmapMgr.h"

void TBitmapMgr::AddCache(int iKey, T_STR name, TBitmap* pData)
{
	TCacheData cache;
	cache.iKey = iKey;
	cache.name = name;
	cache.m_pBitmap = pData;

	if (m_mapCacheList.size() >= 3)
	{
		m_mapCacheList.pop_front();
	}

	m_mapCacheList.push_back(cache);
}
  
int TBitmapMgr::GetCache(T_STR szName)
{
	std::list<TCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TCacheData& cache = (*iter);
		if (cache.name == szName)
		{
			return cache.iKey;
		}
	}

	return -1;
}

TBitmap* TBitmapMgr::GetCache(int iKey)
{
	std::list<TCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TCacheData& cache = (*iter);
		if (cache.iKey == iKey)
		{
			return cache.m_pBitmap;
		}
	}

	return NULL;
}

int TBitmapMgr::Load(T_STR szLoadFile)
{
	T_STR szName = szLoadFile;
	int iKey = GetCache(szName);
	if (iKey > 0)
	{
		return iKey;
	}

	std::map<int, TBitmap*>::iterator iter;
	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		TBitmap* pBitmap = (TBitmap*)(*iter).second;
		if (pBitmap->m_szName == szName)
		{
			AddCache((*iter).first, szName, pBitmap);
			return (*iter).first;
		}
	}
	TBitmap* pBitmap1 = new TBitmap;
	if (pBitmap1->Load(szLoadFile))
	{
		m_mapList.insert(make_pair(++m_iIndex, pBitmap1));
		AddCache(m_iIndex, szName, pBitmap1);
		return m_iIndex;
	}
	
	return -1;
}
TBitmap* TBitmapMgr::GetPtr(int iKey)
{
	TBitmap* pBitmap = GetCache(iKey);
	if (pBitmap != NULL)
	{
		return pBitmap;
	}

	std::map<int, TBitmap*>::iterator iter;

	iter = m_mapList.find(iKey);
	if (iter == m_mapList.end())
		return NULL;

	return (*iter).second;
}
TBitmapMgr::TBitmapMgr()
{
	m_iIndex = -1;
}


TBitmapMgr::~TBitmapMgr()
{
}
