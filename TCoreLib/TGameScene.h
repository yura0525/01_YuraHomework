#pragma once
#include "TScene.h"
#include "TObject.h"
#include "TNPCObject.h"
#include "TBulletObject.h"
#include "THeroObject.h"
#include <vector>
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
	vector<TNPCObject>	m_NPCList;

	float						m_fAngle;
	std::vector<TBulletObject>	m_NPCBulletList;
	std::vector<TBulletObject>	m_HeroBulletList;
public:
	int							m_NPCGap;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool Reset();

public:
	std::vector<RECT_ARRAY> m_rtSpriteList;
	int						m_iSpriteIndex;
public:

	bool GameDataLoad(const TCHAR* pszFileName);

	void AddBullet(eBulletType eType, POINT pos, float dirX, float dirY, float speed);

public:
	TGameScene();
	virtual ~TGameScene();
};

