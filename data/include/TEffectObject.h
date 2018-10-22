#pragma once
#include "TNPCObject.h"
#include <vector>

typedef std::vector<RECT> RECT_ARRAY;

class TEffectObject : public TNPCObject
{
public:
	int		m_iCurrentSprite;
	int		m_iIndexSprite;

	float	m_fSpriteTime;
	float	m_fOffSet;
	float	m_fLifeTime;
	
public:
	bool Frame();
public:
	TEffectObject();
	virtual ~TEffectObject();
};

class TEffectMgr : public TSingleton<TEffectMgr>
{
public:
	float						m_fAngle;
	std::vector<TEffectObject>	m_effectObjList;
	std::vector<RECT_ARRAY>		m_rtSpriteList;
	
public:
	bool GameDataLoad(const TCHAR* pszFileName);
	void AddEffect(POINT pos);
	bool IsCollision(RECT rt);
	
public:
	bool Frame();
	bool Render();
	bool Release();
public:
	TEffectMgr();
	~TEffectMgr();
};


#define I_EffectMgr TEffectMgr::GetInstance()