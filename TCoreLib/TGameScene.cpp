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
	}

	//NPCBulletList
	m_NPCBulletList.resize(g_iMaxBulletCount);
	for (int iNPC = 0; iNPC < m_NPCBulletList.size(); iNPC++)
	{
		m_NPCBulletList[iNPC].Init();
		m_NPCBulletList[iNPC].SetSpriteList(m_rtSpriteList);
		m_NPCBulletList[iNPC].SetBulletDirectionSpeed(0, -1, 100.0f);
		m_NPCBulletList[iNPC].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
		m_NPCBulletList[iNPC].m_bDead = true;
	}

	//HeroBulletList
	m_HeroBulletList.resize(g_iMaxBulletCount);
	for (int iHero = 0; iHero < m_HeroBulletList.size(); iHero++)
	{
		m_HeroBulletList[iHero].Init();
		m_HeroBulletList[iHero].SetSpriteList(m_rtSpriteList);
		m_HeroBulletList[iHero].SetBulletDirectionSpeed(0, 1, 100.0f);
		m_HeroBulletList[iHero].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
		m_HeroBulletList[iHero].m_bDead = true;
	}

	GameDataLoad(L"../../data/SpriteList.txt");
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
		for (int iObj = 0; iObj < m_HeroBulletList.size(); iObj++)
		{
			if (TCollision::RectInRect(m_NPCList[iNPC].m_rtCollision, m_HeroBulletList[iObj].m_rtCollision))
				m_NPCList[iNPC].ProcessDamage(-1);

			m_HeroBulletList[iObj].Frame();
		}
		if (!m_NPCList[iNPC].m_bDead)
			m_NPCList[iNPC].Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(m_Hero.m_rtCollision, m_NPCList[iNPC].m_rtCollision))
			m_Hero.ProcessDamage(-1);
	}

	
	
	/*bool isChangeScene = true;
	for (int iNPC = 0; iNPC < m_iMaxNPCCount; iNPC++)
	{
		if ( I_Input.Key(VK_LBUTTON) &&
			TCollision::RectInPoint(m_NPCList[iNPC].m_rtCollision, I_Input.m_MousePos))
		{
			m_NPCList[iNPC].m_bDead = true;
		}
		
		if( !m_NPCList[iNPC].m_bDead )
			isChangeScene = false;
	}

	if (isChangeScene)
	{
		m_bNextSceneStart = true;
	}*/

	if (I_Input.Key(VK_LBUTTON) == KEY_PUSH)
	{
		static float fAddTime = 0.0f;
		fAddTime += g_fSecPerFrame;

		if (fAddTime >= 0.003f);
		{
			POINT pos;
			pos.x = m_Hero.m_pos.x;
			pos.x = m_Hero.m_pos.y;

			AddBullet(eHeroBullet, pos, 0.0f, 1.0f, 100.0f);
			fAddTime -= 0.003f;
		}
	}

	//주인공이 NPC총알에 맞는거 처리.
	for (int iObj = 0; iObj < m_NPCBulletList.size(); iObj++)
	{
		m_NPCBulletList[iObj].Frame();
		if (TCollision::RectInRect(m_Hero.m_rtCollision, m_NPCBulletList[iObj].m_rtCollision))
			m_Hero.ProcessDamage(-1);
	}


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

	m_fAngle += g_fSecPerFrame * 100.0f;

	for (int iObj = 0; iObj < m_NPCBulletList.size(); iObj++)
	{
		m_NPCBulletList[iObj].RotationBlt(m_fAngle);
	}

	for (int iObj = 0; iObj < m_HeroBulletList.size(); iObj++)
	{
		m_HeroBulletList[iObj].RotationBlt(m_fAngle);
	}

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

	for (int iObj = 0; iObj < m_NPCBulletList.size(); iObj++)
	{
		m_NPCBulletList[iObj].Release();
	}

	for (int iObj = 0; iObj < m_HeroBulletList.size(); iObj++)
	{
		m_HeroBulletList[iObj].Release();
	}

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

bool TGameScene::GameDataLoad(const TCHAR* pszFileName)
{
	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumSprite = 0;
	FILE* fp_src;

	_wfopen_s(&fp_src, pszFileName, _T("rt"));

	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);

	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp), &rt.left, &rt.top, &rt.right, &rt.bottom);

			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}

void TGameScene::AddBullet(eBulletType eType, POINT pos, float dirX, float dirY, float speed)
{
	TBulletObject* obj = NULL;
	
	switch (eType)
	{
	case eNPCBullet:
		for (int iNPC = 0; iNPC < m_NPCBulletList.size(); iNPC++)
		{
			if (m_NPCBulletList[iNPC].m_bDead)
			{
				obj = &(m_NPCBulletList[iNPC]);
				obj->m_bDead = false;
				break;
			}
		}
		break;
	case eHeroBullet:
		for (int iHero = 0; iHero < m_HeroBulletList.size(); iHero++)
		{
			if (m_HeroBulletList[iHero].m_bDead)
			{
				obj = &(m_HeroBulletList[iHero]);
				obj->m_bDead = false;
				break;
			}
		}
		break;
	}

	if (obj == NULL)
	{
		TCHAR m_csBuffer[256];
		_stprintf_s(m_csBuffer, L"obj == NULL!!!!!!!!!!!!!!!!!!!!!",
			eType, pos.x, pos.y, dirX, dirY, speed);
		OutputDebugString(m_csBuffer);

		obj = new TBulletObject();
		m_HeroBulletList.push_back(*obj);
		obj = &(m_HeroBulletList.at(m_HeroBulletList.size()-1));
		return;
	}

	obj->SetPosition(pos.x, pos.y, 1, 142, 41, 42);
	obj->SetBulletDirectionSpeed(dirX, dirY, speed);

	if (!m_rtSpriteList.empty())
		obj->m_iIndexSprite = rand() % m_rtSpriteList.size();

	TCHAR m_csBuffer[256];
	/*_stprintf_s(m_csBuffer, L"eType = %d, pos.x : %f, pos.y : %f, dirX : %f, dirY : %f, speed : %f", 
		eType, pos.x, pos.y, dirX, dirY, speed);
	OutputDebugString(m_csBuffer);*/
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
