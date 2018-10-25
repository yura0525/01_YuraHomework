#include "TGameScene.h"
#include "xInput.h"
#include "TCollision.h"

const int g_NPCWidthGap = 55;
const int g_HeroMAXHP = 100;
const int g_NPCMoveSpeed = 100.0f;
const int g_NPCYPos = 50.0f;
bool TGameScene::Init()
{
	m_bNextSceneStart = false;
	m_iSceneID = GAME_SCENE_PLAY;
	m_fAngle = 0.0f;
	
	m_bEndSceneStart = false;

	I_EffectMgr.GameDataLoad(L"../data/Resource/SpriteList.txt");

	m_BackGround = make_shared<TBKScrollObject>();
	m_BackGround->Create(g_pd3dDevice, 450, 600, (g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600,
		L"vertexshader.txt", L"../data/Resource/background_00.png");

	m_Hero = make_shared<THeroObject>();
	m_Hero->Create(g_pd3dDevice, 150, 100, 300, 300, 0, 0, 150, 100, L"vertexshader.txt", L"../data/Resource/Hero.png");

	int NPCXPos = 0;
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		NPCXPos = (m_NPCGap / 2) + (m_NPCGap * iNPC);

		pNPCObject->Create(g_pd3dDevice, 347, 201, NPCXPos, g_NPCYPos, 87, 0, 174, 100, L"vertexshader.txt", L"../data/Resource/dragon.png");
		pNPCObject->SetMAXHP(1);

		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
	return true;
}

bool TGameScene::Frame()
{
	if (I_Input.m_KeyState[DIK_F1] )
	{
		m_Hero->m_bDebugRect = !(m_Hero->m_bDebugRect);

		list<TNPCObject*>::iterator iter;
		for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
		{
			(*iter)->m_bDebugRect = !( (*iter)->m_bDebugRect );
		}
	}
	m_BackGround->Frame();
	m_Hero->Frame();

	list<TNPCObject*>::iterator iter;
	int iNPC = 0;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++, iNPC++)
	{
		TNPCObject* pNPCObject = (*iter);
		//NPC가 Hero의 총알에 맞는 처리
		if (I_EffectMgr.IsCollision(pNPCObject->m_rtCollision))
		{
			pNPCObject->ProcessDamage(-1);
		}
		
		pNPCObject->Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(m_Hero->m_rtCollision, pNPCObject->m_rtCollision))
		{
			m_Hero->ProcessDamage(-1);
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
	if (m_Hero->IsDead())
		m_bEndSceneStart = true;

	return true;
}

bool TGameScene::Render()
{
	m_BackGround->Render();
	m_Hero->Render();

	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		(*iter)->Render();
	}

	I_EffectMgr.Render();
	m_fAngle += g_fSecPerFrame * 100.0f;

	return true;
}
bool TGameScene::Release()
{
	m_BackGround->Release();
	m_Hero->Release();

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

	m_Hero->SetPosition((g_rtClient.right / 2), (g_rtClient.bottom / 2));
	m_Hero->SetMAXHP(g_HeroMAXHP);

	//NPCList
	int iNPC = 0, NPCXPos = 0;
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++, iNPC++)
	{
		TNPCObject* pNPCObject = (*iter);
		NPCXPos = (m_NPCGap / 2) + (m_NPCGap * iNPC);

		pNPCObject->SetPosition(NPCXPos, g_NPCYPos);
		pNPCObject->SetMAXHP(1);
		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
	}

	return true;
}

void TGameScene::DeleteNPCList()
{
	list<TNPCObject*>::iterator iter;
	int iNPC = 0;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); )
	{
		TNPCObject* pNPCObject = (*iter);
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
	int NPCXPos = 0;
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		NPCXPos = (m_NPCGap / 2) + (m_NPCGap * iNPC);

		pNPCObject->Create(g_pd3dDevice, 347, 201, NPCXPos, g_NPCYPos, 87, 0, 174, 100, L"vertexshader.txt", L"../data/Resource/dragon.png");
		pNPCObject->SetMAXHP(1);

		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
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
