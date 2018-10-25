#pragma once
#include "xStd.h"
#include "TNPCObject.h"
typedef std::vector<RECT> RECT_ARRAY;

class TEffectObject : public TNPCObject
{
public:
	int		m_iCurrentSprite;
	int		m_iIndexSprite;

	float	m_fSpriteTime;
	float	m_fLifeTime;
	float	m_fOffSet;

public:
	bool Frame();
	void SpriteRender();

public:
	TEffectObject();
	virtual ~TEffectObject();
};

class TEffectMgr : public TSingleton<TEffectMgr>
{
public:
	friend class TSingleton<TEffectMgr>;
	float							m_fAngle;
	std::vector<RECT_ARRAY>			m_rtSpriteList;

	std::list<TEffectObject*>		m_effectObjList;
public:
	bool GameDataLoad(const TCHAR* pszFileName);
	void AddEffect(POINT pos);
	bool IsCollision(RECT rt);

public:
	bool Frame();
	bool Render();
	bool Release();
	void DeleteEffectList();

protected:
	TEffectMgr();
public:
	~TEffectMgr();
};

#define I_EffectMgr TEffectMgr::GetInstance()