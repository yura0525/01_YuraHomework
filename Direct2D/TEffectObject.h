#pragma once
#include "xStd.h"
#include "xObject.h"
typedef std::vector<RECT> RECT_ARRAY;
class TNPCObject;
class TItemObject;
class TEffectObject : public xObject
{
public:
	int			m_iCurrentSprite;
	int			m_iIndexSprite;

	float		m_fSpriteTime;
	float		m_fLifeTime;
	float		m_fOffSet;
	bool		m_IsHeroEffect;
public:
	bool Init();
	bool Frame();

public:
	TEffectObject();
	virtual ~TEffectObject();
};


class TEffectMgr : public TSingleton<TEffectMgr>
{
public:
	friend class TSingleton<TEffectMgr>;
	std::vector<RECT_ARRAY>			m_rtSpriteList;

	std::list<TEffectObject*>		m_effectObjListByHero;	//Hero총알
	std::list<TEffectObject*>		m_effectObjListByNPC;	//불구덩이 같은 NPC공격

	std::list<TItemObject*>			m_ItemObjList;			//생성된 아이템.
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
	void AddEffectByNPC(float xPos, float yPos);
	void AddItemByHeroEffect(float xPos, float yPos);

	bool IsCollisionAndDeleteList(RECT rt);
	void DeleteEffectList();

	void NPCEffectRegenAlarm();
protected:
	TEffectMgr();
public:
	~TEffectMgr();
};

#define I_EffectMgr TEffectMgr::GetInstance()