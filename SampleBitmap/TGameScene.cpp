#include "TGameScene.h"
#include "TInput.h"
#include "TCollision.h"

bool TGameScene::Init()
{
	m_bNextSceneStart = false;
	m_iSceneID = GAME_SCENE_PLAY;
	m_fAngle = 0.0f;
	
	m_NPCGap = 55;
	m_bEndSceneStart = false;

	m_EffectMgr.GameDataLoad(L"../../data/SpriteList.txt");


	m_BackGround.Init();
	m_BackGround.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/bk.bmp");

	m_Hero.Init();
	m_Hero.SetPosition(300, 300, 90, 2, 42, 58);
	m_Hero.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	m_Hero.SetHP(10);

	//NPCList
	m_NPCList.resize(g_iMaxNPCCount);
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		m_NPCList[iNPC].Init();
		m_NPCList[iNPC].SetPosition(0 + (m_NPCGap * iNPC), 0, 46, 62, 68, 82);
		
		m_NPCList[iNPC].SetHP(1);
		m_NPCList[iNPC].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");	
		m_NPCList[iNPC].m_fAttackRadius = 30 + rand() % 100;
		m_NPCList[iNPC].SetDirectionSpeed(0.0f, 1.0f, 50.0f);
	}

	m_iSpriteIndex = 0;
	return true;
}

bool TGameScene::Frame()
{
	if (I_Input.Key('0') == KEY_PUSH)
	{
		m_Hero.m_bDebugRect = !m_Hero.m_bDebugRect;
		for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
		{
			m_NPCList[iNPC].m_bDebugRect = !m_NPCList[iNPC].m_bDebugRect;
		}
	}
	m_BackGround.Frame();
	m_Hero.Frame();

	
	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		//NPC가 Hero의 총알에 맞는 처리
		if (I_EffectMgr.IsCollision(m_NPCList[iNPC].m_rtCollision))
		{
			m_NPCList[iNPC].ProcessDamage(-1);
		}

		if (!m_NPCList[iNPC].m_bDead)
			m_NPCList[iNPC].Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(m_Hero.m_rtCollision, m_NPCList[iNPC].m_rtCollision))
			m_Hero.ProcessDamage(-1);
	}

	m_EffectMgr.Frame();
	
	bool isChangeScene = true;
	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		/*if ( I_Input.Key(VK_LBUTTON) &&
			TCollision::RectInPoint(m_NPCList[iNPC].m_rtCollision, g_pHeroPos))
		{
			m_NPCList[iNPC].m_bDead = true;
		}
		*/
		if( !m_NPCList[iNPC].m_bDead )
			isChangeScene = false;
	}

	if (isChangeScene)
	{
		//몹 재생성.
		for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
		{
			m_NPCList[iNPC].Init();
			m_NPCList[iNPC].SetPosition(0 + (m_NPCGap * iNPC), 0, 46, 62, 68, 82);

			m_NPCList[iNPC].SetHP(1);
			m_NPCList[iNPC].m_fAttackRadius = 30 + rand() % 100;
			m_NPCList[iNPC].SetDirectionSpeed(0.0f, 1.0f, 50.0f);
		}
	}

	////주인공이 NPC총알에 맞는거 처리.
	//for (int iObj = 0; iObj < m_NPCBulletList.size(); iObj++)
	//{
	//	m_NPCBulletList[iObj].Frame();
	//	if (TCollision::RectInRect(m_Hero.m_rtCollision, m_NPCBulletList[iObj].m_rtCollision))
	//		m_Hero.ProcessDamage(-1);
	//}


	//주인공이 죽으면 게임끝
	if (m_Hero.m_bDead)
		m_bEndSceneStart = true;

	return true;
}

bool TGameScene::Render()
{
	m_BackGround.Render();
	m_Hero.Render();
	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		if( !m_NPCList[iNPC].m_bDead )
			m_NPCList[iNPC].Render();
	}

	m_EffectMgr.Render();
	m_fAngle += g_fSecPerFrame * 100.0f;

	return true;
}
bool TGameScene::Release()
{
	m_BackGround.Release();
	m_Hero.Release();
	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		m_NPCList[iNPC].Release();
	}
	m_EffectMgr.Release();
	return true;
}
bool TGameScene::Reset()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_bEndSceneStart = false;

	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		m_NPCList[iNPC].m_bDead = false;
		m_NPCList[iNPC].SetPosition(0 + (m_NPCGap * iNPC), 0, 46, 62, 68, 82);
	}
	return true;
}

void TGameScene::AddBullet(eBulletType eType, POINT pos, float dirX, float dirY, float speed)
{
	
}

TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_fAngle = 0.0f;

	//NPC사이의 갭들
	m_NPCGap = 55;
	m_bEndSceneStart = false;
}


TGameScene::~TGameScene()
{
}
