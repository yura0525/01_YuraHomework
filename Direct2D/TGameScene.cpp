#include "TGameScene.h"
#include "xInput.h"
#include "TCollision.h"
#include "TGameDataLoad.h"

bool TGameScene::Init()
{
	m_bNextSceneStart = false;
	m_iSceneID = GAME_SCENE_PLAY;
	
	m_bEndSceneStart = false;

	I_GameDataLoad.Init();

	m_BackGround.Create(g_pd3dDevice, 450, 600, (g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600,
		L"vertexshader.txt", L"../data/Resource/background_00.png");

	I_HeroMgr.Init();
	I_NPCMgr.Init();
	I_EffectMgr.Init();

	return true;
}

bool TGameScene::Frame()
{
	m_BackGround.Frame();

	//주인공이 죽으면 게임끝
	if (I_HeroMgr.m_Hero.IsDead())
		m_bNextSceneStart = true;

	I_HeroMgr.Frame();
	I_NPCMgr.Frame();
	I_EffectMgr.Frame();

	return true;
}

bool TGameScene::Render()
{
	m_BackGround.Render();
	I_HeroMgr.Render();
	I_NPCMgr.Render();
	I_EffectMgr.Render();

	return true;
}
bool TGameScene::Release()
{
	m_BackGround.Release();
	I_HeroMgr.Release();
	I_NPCMgr.Release();
	I_EffectMgr.Release();

	return true;
}
bool TGameScene::Reset()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_bEndSceneStart = false;

	I_HeroMgr.Reset();

	return true;
}



TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;

	m_bEndSceneStart = false;
}


TGameScene::~TGameScene()
{
}
