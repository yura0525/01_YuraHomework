#include "xTexManager.h"

void xTexManager::AddCache(int iKey, T_STR name, xTexture* pData)
{
	TTextureCacheData cache;
	cache.iKey = iKey;
	cache.name = name;
	cache.m_pBitmap = pData;

	if (m_mapCacheList.size() >= 3)
	{
		m_mapCacheList.pop_front();
	}

	m_mapCacheList.push_back(cache);
}

int xTexManager::GetCache(T_STR szName)
{
	std::list<TTextureCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TTextureCacheData& cache = (*iter);
		if (cache.name == szName)
		{
			return cache.iKey;
		}
	}

	return -1;
}

xTexture* xTexManager::GetCache(int iKey)
{
	std::list<TTextureCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TTextureCacheData& cache = (*iter);
		if (cache.iKey == iKey)
		{
			return cache.m_pBitmap;
		}
	}

	return NULL;
}

int xTexManager::Load(ID3D11Device *pd3dDevice, T_STR szLoadFile)
{
	T_STR szName = szLoadFile;
	m_pd3dDevice = pd3dDevice;
	int iKey = GetCache(szName);
	if (iKey > 0)
	{
		return iKey;
	}

	std::map<int, xTexture*>::iterator iter;
	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		xTexture* pBitmap = (xTexture*)(*iter).second;
		if (pBitmap->m_szName == szName)
		{
			AddCache((*iter).first, szName, pBitmap);
			return (*iter).first;
		}
	}
	xTexture* pBitmap1 = new xTexture;
	if (pBitmap1->Load(m_pd3dDevice, szLoadFile))
	{
		m_mapList.insert(make_pair(++m_iIndex, pBitmap1));
		AddCache(m_iIndex, szName, pBitmap1);
		return m_iIndex;
	}

	return -1;
}
xTexture* xTexManager::GetPtr(int iKey)
{
	xTexture* pBitmap = GetCache(iKey);
	if (pBitmap != NULL)
	{
		return pBitmap;
	}

	std::map<int, xTexture*>::iterator iter;

	iter = m_mapList.find(iKey);
	if (iter == m_mapList.end())
		return NULL;

	return (*iter).second;
}

bool xTexManager::Release()
{
	std::map<int, xTexture*>::iterator iter;

	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		xTexture* pBitmap = (xTexture*)(*iter).second;
		pBitmap->Release();
		delete pBitmap;
	}

	m_mapList.clear();
	return true;
}
xTexManager::xTexManager()
{
	m_iIndex = -1;
}


xTexManager::~xTexManager()
{
}
