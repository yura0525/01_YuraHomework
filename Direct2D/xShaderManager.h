#pragma once
#include "xShader.h"
using namespace std;

struct TShaderCacheData
{
	int			iKey;
	T_STR		name;
	xShader*	m_pBitmap;
};

class xShaderManager : public TSingleton<xShaderManager>
{
public:
	friend class TSingleton<xShaderManager>;

	std::map<int, xShader*> m_mapList;
	std::list<TShaderCacheData> m_mapCacheList;

	int m_iIndex;
	ID3D11Device *			m_pd3dDevice;	// 디바이스 객체

public:
	int Load(ID3D11Device *pd3dDevice, T_STR szLoadFile);
	xShader* GetPtr(int iKey);

	void AddCache(int iKey, T_STR name, xShader* pData);
	int GetCache(T_STR szName);
	xShader* GetCache(int iKey);
	bool Release();

protected:
	xShaderManager();
public:
	virtual ~xShaderManager();
};

#define I_ShaderMgr xShaderManager::GetInstance()

