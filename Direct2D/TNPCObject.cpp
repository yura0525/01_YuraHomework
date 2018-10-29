#include "TNPCObject.h"
#include "THeroObject.h"
#include "TEffectObject.h"

const int g_iMaxNPCCount = 15;
const int g_NPCMoveSpeed = 150.0f;
const int g_NPCWidthGap = 55;
const int g_NPCYPos = 50.0f;

bool TNPCObject::Init()
{
	m_fSpeed = g_NPCMoveSpeed;			//NPC 이동 속도
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
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

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	//위치값이나 충돌박스 수정.
	return xObject::Frame();
}

TNPCObject::TNPCObject()
{
	m_fSpeed = g_NPCMoveSpeed;			//NPC 이동 속도
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
}


TNPCObject::~TNPCObject()
{
}

bool TNPCMgr::Init()
{
	//SpriteDataLoad(L"../data/Resource/monsterList.txt");
	int NPCXPos = 0;
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		NPCXPos = (g_NPCWidthGap / 2) + (g_NPCWidthGap * iNPC);

		pNPCObject->Create(g_pd3dDevice, 347, 201, NPCXPos, g_NPCYPos, 87, 0, 174, 100, L"vertexshader.txt", L"../data/Resource/dragon.png");
		pNPCObject->SetMAXHP(1);

		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
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
		if (I_EffectMgr.IsCollision(pNPCObject->m_rtCollision))
		{
			pNPCObject->ProcessDamage(-1);
		}

		pNPCObject->Frame();

		//Hero가 NPC에 맞는거 처리.
		if (TCollision::RectInRect(I_HeroMgr.m_Hero.m_rtCollision, pNPCObject->m_rtCollision))
		{
			I_HeroMgr.m_Hero.ProcessDamage(-1);
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
		NPCXPos = (g_NPCWidthGap / 2) + (g_NPCWidthGap * iNPC);

		pNPCObject->SetPosition(NPCXPos, g_NPCYPos);
		pNPCObject->SetMAXHP(1);
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
	for (int iNPC = 0; iNPC < g_iMaxNPCCount; iNPC++)
	{
		TNPCObject* pNPCObject = new TNPCObject;
		NPCXPos = (g_NPCWidthGap / 2) + (g_NPCWidthGap * iNPC);

		pNPCObject->Create(g_pd3dDevice, 347, 201, NPCXPos, 50.0f, 87, 0, 174, 100, L"vertexshader.txt", L"../data/Resource/dragon.png");
		pNPCObject->SetMAXHP(1);

		//pNPCObject->m_fAttackRadius = 30 + rand() % 100;
		//pNPCObject->SetDirectionSpeed(0.0f, 1.0f, g_NPCMoveSpeed);
		m_NPCList.push_back(pNPCObject);
	}
}

bool TNPCMgr::SpriteDataLoad(const TCHAR* pszFileName)
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

TNPCMgr::TNPCMgr()
{

}

TNPCMgr::~TNPCMgr()
{

}