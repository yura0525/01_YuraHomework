#include "TNPCObject.h"
#include "THeroObject.h"
#include "TEffectObject.h"
#include "THPBarObject.h"

bool TNPCObject::Init()
{
	m_fSpeed = g_MOVE_NPC_SPEED;			//NPC 이동 속도
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;

	m_HPBar.SetOwner(this);
	m_HPBar.Init();
	
	return xObject::Init();
}

bool TNPCObject::Frame()
{
	if (IsDead())
		return false;

	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y - iHalfY) >= g_rtClient.bottom)
	{
		SetDead();
		return false;
	}

	m_HPBar.Frame();

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	//위치값이나 충돌박스 수정.
	return xObject::Frame();
}

bool TNPCObject::Render()
{
	m_HPBar.Render();
	return xObject::Render();
}

bool TNPCObject::Release()
{
	m_HPBar.Release();
	return xObject::Release();
}

TNPCObject::TNPCObject(eNPCTYPE eType) : m_HPBar(NULL)
{
	m_eNPCType = eType;
	m_fSpeed = g_MOVE_NPC_SPEED;			//NPC 이동 속도
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
}


TNPCObject::~TNPCObject()
{
}

bool TNPCMgr::Init()
{
	SpriteDataLoad(L"../data/Resource/NPCList.txt");
	return true;
}

bool TNPCMgr::Frame()
{
	list<TNPCObject*>::iterator iter;
	int iNPC = 0;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++, iNPC++)
	{
		TNPCObject* pNPCObject = (*iter);

		//NPC가 Hero의 총알에 맞는 처리
		if (I_EffectMgr.IsCollisionAndDeleteList(pNPCObject->m_rtCollision, true))
		{
			pNPCObject->ProcessDamage(-1);
		}

		pNPCObject->Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(I_HeroMgr.m_Hero.m_rtCollision, pNPCObject->m_rtCollision))
		{
			I_HeroMgr.ProcessDamage(-1);
		}
	}

	DeleteNPCList();
	return true;
}

bool TNPCMgr::Render()
{
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		(*iter)->Render();
	}
	return true;
}

bool TNPCMgr::Release()
{
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++)
	{
		(*iter)->Release();
		delete (*iter);
	}
	m_NPCList.clear();
	return true;
}

void TNPCMgr::Reset()
{
	//NPCList
	int iNPC = 0, NPCXPos = 0;
	list<TNPCObject*>::iterator iter;
	for (iter = m_NPCList.begin(); iter != m_NPCList.end(); iter++, iNPC++)
	{
		TNPCObject* pNPCObject = (*iter);
		NPCXPos = g_INIT_NPC_POSX + (g_NPC_WIDTH_GAP / 2) + (g_NPC_WIDTH_GAP * iNPC);

		pNPCObject->SetPosition(NPCXPos, g_INIT_NPC_POSY);
		pNPCObject->SetMAXHP(pNPCObject->m_eNPCType);

		pNPCObject->m_HPBar.SetPosition(NPCXPos, (g_INIT_NPC_POSY + g_INIT_HERO_HP_POSY));
		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
	}
}
void TNPCMgr::DeleteNPCList()
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

void TNPCMgr::NPCRegenAlarm()
{
	//몹 재생성.
	int NPCXPos = 0;
	srand(time(NULL));

	for (int iNPC = 0; iNPC < g_iMAX_NPC_COUNT; iNPC++)
	{
		//eNPCTYPE으로 HP값을 셋팅하므로 1부터 하게 했다.
		eNPCTYPE eType = (eNPCTYPE)(rand() % eMaxDragon);
		TNPCObject* pNPCObject = new TNPCObject(eType);

		if (m_rtSpriteList.empty())
			return;

		pNPCObject->Init();

		RECT rt = m_rtSpriteList[0][eType];
		NPCXPos = g_INIT_NPC_POSX + (g_NPC_WIDTH_GAP / 2) + (g_NPC_WIDTH_GAP * iNPC);

		pNPCObject->Create(g_pd3dDevice, 609, 100, NPCXPos, g_INIT_NPC_POSY,
			rt.left, rt.top, rt.right, rt.bottom,
			L"vertexshader.txt", L"../data/Resource/dragon.png");
		pNPCObject->SetMAXHP(eType + 1);

		pNPCObject->m_HPBar.Create(g_pd3dDevice, 100, 27, NPCXPos, (g_INIT_NPC_POSY + g_INIT_HERO_HP_POSY),
			0, 0, 100, 27, L"vertexshader.txt", L"../data/Resource/HPBK.png");

		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
}

bool TNPCMgr::SpriteDataLoad(const TCHAR* pszFileName)
{
	m_rtSpriteList.clear();

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

		m_rtSpriteList[iCnt].clear();

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

TNPCMgr::TNPCMgr()
{

}

TNPCMgr::~TNPCMgr()
{

}