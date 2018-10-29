#pragma once
#include "xStd.h"
#include "xObject.h"
typedef std::vector<RECT> RECT_ARRAY;

class TEffectObject : public xObject
{
public:
	xObject*	m_pOwner;
	int			m_iCurrentSprite;
	int			m_iIndexSprite;

	float		m_fSpriteTime;
	float		m_fLifeTime;
	float		m_fOffSet;

public:
	bool Init();
	bool Frame();

public:
	TEffectObject(xObject*	pOwner);
	virtual ~TEffectObject();
};

class TNPCObject;
class TEffectMgr : public TSingleton<TEffectMgr>
{
public:
	friend class TSingleton<TEffectMgr>;
	std::vector<RECT_ARRAY>			m_rtSpriteList;

	std::list<TEffectObject*>		m_effectObjListByHero;
	std::list<TEffectObject*>		m_effectObjListByNPC;
public:
	float							m_fAngle;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	
public:
	bool SpriteDataLoad(const TCHAR* pszFileName);
	void AddEffectByHero();
	void AddEffectByNPC(TNPCObject* pOwner);
	bool IsCollisionAndDeleteList(RECT rt, bool isHeroEffect = true);
	void DeleteEffectList();

protected:
	TEffectMgr();
public:
	~TEffectMgr();
};

#define I_EffectMgr TEffectMgr::GetInstance()