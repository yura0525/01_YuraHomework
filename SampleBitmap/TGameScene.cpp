#include "TGameScene.h"
#include "TInput.h"
#include "TCollision.h"

const int g_NPCWidthGap = 55;
const int g_HeroMAXHP = 100;
const int g_NPCMoveSpeed = 100.0f;
bool TGameScene::Init()
{
	m_bNextSceneStart = false;
	m_iSceneID = GAME_SCENE_PLAY;
	m_fAngle = 0.0f;
	
	m_NPCGap = 55;
	m_bEndSceneStart = false;

	I_EffectMgr.SpriteDataLoad(L"../data/Resource/SpriteList.txt");

	m_BackGround.Init();
	m_BackGround.SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../data/Resource/bk.bmp");

	m_Hero.Init();
	m_Hero.SetPosition(300, 300, 90, 2, 42, 58);
	m_Hero.Load(L"../data/Resource/bitmap1.bmp", L"../data/Resource/bitmap2.bmp");
	m_Hero.SetMAXHP(g_HeroMAXHP);

	//NPCList
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		pNPCObject->Init();
		pNPCObject->SetPosition((m_NPCGap / 2) + (m_NPCGap * iNPC), 0, 46, 62, 68, 80);
		
		pNPCObject->SetMAXHP(1);
		pNPCObject->Load(L"../data/Resource/bitmap1.bmp", L"../data/Resource/bitmap2.bmp");
		pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
	return true;
}

bool TGameScene::Frame()
{
	if (I_Input.Key('0') == KEY_PUSH)
	{
		m_Hero.m_bDebugRect = !m_Hero.m_bDebugRect;

		list<TNPCObject*>::iterator iter;
		for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
		{
			(*iter)->m_bDebugRect = !( (*iter)->m_bDebugRect );
		}
	}
	m_BackGround.Frame();
	m_Hero.Frame();

	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		TNPCObject* pNPCObject = (*iter);
		//NPC가 Hero의 총알에 맞는 처리
		if (I_EffectMgr.IsCollision(pNPCObject->m_rtCollision))
		{
			pNPCObject->ProcessDamage(-1);
		}

		if (!(pNPCObject->IsDead()))
			pNPCObject->Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(m_Hero.m_rtCollision, pNPCObject->m_rtCollision))
		{
			m_Hero.ProcessDamage(-1);
		}
			
	}

	DeleteNPCList();

	I_EffectMgr.Frame();

	////주인공이 NPC총알에 맞는거 처리.
	//for (int iObj = 0; iObj < m_NPCBulletList.size(); iObj++)
	//{
	//	m_NPCBulletList[iObj].Frame();
	//	if (TCollision::RectInRect(m_Hero.m_rtCollision, m_NPCBulletList[iObj].m_rtCollision))
	//		m_Hero.ProcessDamage(-1);
	//}


	//주인공이 죽으면 게임끝
	if (m_Hero.IsDead())
		m_bEndSceneStart = true;

	return true;
}

bool TGameScene::Render()
{
	m_BackGround.Render();
	m_Hero.Render();
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		if( !((*iter)->IsDead()) )
			(*iter)->Render();
	}

	I_EffectMgr.Render();
	m_fAngle += g_fSecPerFrame * 100.0f;

	return true;
}
bool TGameScene::Release()
{
	m_BackGround.Release();
	m_Hero.Release();

	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		(*iter)->Release();
		delete (*iter);
	}
	m_NPCList.clear();

	I_EffectMgr.Release();
	return true;
}
bool TGameScene::Reset()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_bEndSceneStart = false;

	list<TNPCObject*>::iterator iter;
	int iNPC = 0;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		TNPCObject* pNPCObject = (*iter);
		pNPCObject->Init();
		pNPCObject->SetPosition((m_NPCGap / 2) + (m_NPCGap * iNPC), 0, 46, 62, 68, 80);
		pNPCObject->SetMAXHP(1);
		pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		pNPCObject->SetPosition((m_NPCGap / 2) + (m_NPCGap * iNPC++), 0, 46, 62, 68, 80);
	}
	return true;
}

void TGameScene::DeleteNPCList()
{
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end();)
	{
		if ((*iter)->IsDead())
		{
			(*iter)->Release();
			delete (*iter);
			iter = m_NPCList.erase(iter);
		}
		else
			iter++;
	}

}

void TGameScene::NPCRegenAlarm()
{
	//몹 재생성.
	list<TNPCObject*>::iterator iter;
	int iNPC = 0;
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		pNPCObject->Init();
		pNPCObject->Load(L"../data/Resource/bitmap1.bmp", L"../data/Resource/bitmap2.bmp");
		pNPCObject->SetMAXHP(1);
		pNPCObject->SetPosition((m_NPCGap / 2) + (m_NPCGap * iNPC), 0, 46, 62, 68, 80);
		pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
}
TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_fAngle = 0.0f;

	//NPC사이의 갭들
	m_NPCGap = g_NPCWidthGap;
	m_bEndSceneStart = false;
}


TGameScene::~TGameScene()
{
}
