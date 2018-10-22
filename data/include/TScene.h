#pragma once
#include "TBkObj.h"
#include "THeroObj.h"
#include "TEnemy.h"
#include "TButton.h"
#include "TEffectObject.h"
#include <vector>
using namespace std;

const int g_iMaxLevel = 10;
const int g_iMaxNpcCount = 10;

enum TGameState
{
	GAME_SCENE_LOBBY = 0,
	GAME_SCENE_PLAY,
	GAME_SCENE_END
};
class TScene
{
public:
	TEffectMgr		m_EffectMgr;
	int				m_iSceneID;

	TBkObj m_BackGround;
	
	bool m_bNextSceneStart;
	int m_iMaxNpcCount;
public:
	virtual bool FadeOut()
	{
		return true;
	}
	virtual bool FadeIn()
	{
		return true;
	}
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release() { return true; }
	virtual bool Reset()	{	return true;	}

	virtual bool IsDead() { return false; }

	virtual bool GetNPCDead(int iNPC) {	return false;	}
	virtual void SetNPCDead(int iNPC, bool bDead = true) { }
	virtual RECT GetCollider(int iNPC)
	{
		RECT rt{ 0,0,0,0 };
		return rt;
	}
	virtual bool SetNPCCount(int iNPC);
public:
	TScene()
	{
		m_bNextSceneStart = false;
		m_iMaxNpcCount = 10;
	}
	virtual ~TScene() {}
};

class TLobbyScene : public TScene
{
public:
	TButton m_btnStart;
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool IsDead() { return m_btnStart.m_bDead; }

	bool FadeOut() { return m_BackGround.FadeOut(); }
	bool FadeIn() { return m_BackGround.FadeIn(); }

public:
	TLobbyScene();
	virtual ~TLobbyScene();
};

class TGameScene : public TScene
{
public:
	THeroObj m_Hero;
	vector<TEnemy> m_npcList;
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Reset();
public:
	bool GetNPCDead(int iNPC);
	void SetNPCDead(int iNPC, bool bDead = true);
	RECT GetCollider(int iNPC);
public:
	TGameScene();
	virtual ~TGameScene();
};


class TEndScene : public TScene
{
public:
	TButton m_btnStart;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool IsDead() { return m_btnStart.m_bDead; }
public:
	TEndScene();
	virtual ~TEndScene();
};