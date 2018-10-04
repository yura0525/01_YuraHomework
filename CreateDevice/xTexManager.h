#pragma once
#include "xTexture.h"
using namespace std;

struct TTextureCacheData
{
	int			iKey;
	T_STR		name;
	xTexture*	m_pBitmap;
};

class xTexManager : public TSingleton<xTexManager>
{
public:
	friend class TSingleton<xTexManager>;

	std::map<int, xTexture*> m_mapList;
	std::list<TTextureCacheData> m_mapCacheList;

	int m_iIndex;
	ID3D11Device *			m_pd3dDevice;	// 디바이스 객체

public:
	int Load(ID3D11Device *pd3dDevice, T_STR szLoadFile);
	xTexture* GetPtr(int iKey);

	void AddCache(int iKey, T_STR name, xTexture* pData);
	int GetCache(T_STR szName);
	xTexture* GetCache(int iKey);
	bool Release();

protected:
	xTexManager();
public:
	virtual ~xTexManager();
};

#define I_TextureMgr xTexManager::GetInstance()

