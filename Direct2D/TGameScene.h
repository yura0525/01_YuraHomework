#pragma once
#include "TScene.h"
#include "TNPCObject.h"
#include "TEffectObject.h"
#include "THeroObject.h"
#include <list>
using namespace std;

typedef std::vector<RECT> RECT_ARRAY;
enum eBulletType
{
	eNPCBullet,
	eHeroBullet,
	eMAX,
};
class TGameScene : public TScene
{
public:
	THeroObject			m_Hero;
	list<TNPCObject*>	m_NPCList;

	float				m_fAngle;
public:
	int					m_NPCGap;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Reset();

public:
	void DeleteNPCList();
	void NPCRegenAlarm();
public:
	TGameScene();
	virtual ~TGameScene();
};

