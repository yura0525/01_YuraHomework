#pragma once
#include "TScene.h"
#include "TBKScrollObject.h"
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
	TBKScrollObject		m_BackGround;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Reset();
public:

	TGameScene();
	virtual ~TGameScene();
};

