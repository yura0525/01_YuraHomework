#include "xShaderManager.h"

void xShaderManager::AddCache(int iKey, T_STR name, xShader* pData)
{
	TShaderCacheData cache;
	cache.iKey = iKey;
	cache.name = name;
	cache.m_pBitmap = pData;

	if (m_mapCacheList.size() >= 3)
	{
		m_mapCacheList.pop_front();
	}

	m_mapCacheList.push_back(cache);
}

int xShaderManager::GetCache(T_STR szName)
{
	std::list<TShaderCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TShaderCacheData& cache = (*iter);
		if (cache.name == szName)
		{
			return cache.iKey;
		}
	}

	return -1;
}

xShader* xShaderManager::GetCache(int iKey)
{
	std::list<TShaderCacheData>::iterator iter;
	for (iter = m_mapCacheList.begin(); iter != m_mapCacheList.end(); iter++)
	{
		TShaderCacheData& cache = (*iter);
		if (cache.iKey == iKey)
		{
			return cache.m_pBitmap;
		}
	}

	return NULL;
}

int xShaderManager::Load(ID3D11Device *pd3dDevice, T_STR szLoadFile)
{
	T_STR szName = szLoadFile;
	m_pd3dDevice = pd3dDevice;
	int iKey = GetCache(szName);
	if (iKey > 0)
	{
		return iKey;
	}

	std::map<int, xShader*>::iterator iter;
	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		xShader* pBitmap = (xShader*)(*iter).second;
		if (pBitmap->m_szName == szName)
		{
			AddCache((*iter).first, szName, pBitmap);
			return (*iter).first;
		}
	}
	xShader* pBitmap1 = new xShader;
	if (pBitmap1->Load(m_pd3dDevice, szLoadFile))
	{
		m_mapList.insert(make_pair(++m_iIndex, pBitmap1));
		AddCache(m_iIndex, szName, pBitmap1);
		return m_iIndex;
	}

	return -1;
}
xShader* xShaderManager::GetPtr(int iKey)
{
	xShader* pBitmap = GetCache(iKey);
	if (pBitmap != NULL)
	{
		return pBitmap;
	}

	std::map<int, xShader*>::iterator iter;

	iter = m_mapList.find(iKey);
	if (iter == m_mapList.end())
		return NULL;

	return (*iter).second;
}

bool xShaderManager::Release()
{
	std::map<int, xShader*>::iterator iter;

	for (iter = m_mapList.begin(); iter != m_mapList.end(); iter++)
	{
		xShader* pBitmap = (xShader*)(*iter).second;
		pBitmap->Release();
		delete pBitmap;
	}

	m_mapList.clear();
	return true;
}
xShaderManager::xShaderManager()
{
	m_iIndex = -1;
}


xShaderManager::~xShaderManager()
{
}
