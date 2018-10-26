#include "TNPCObject.h"

bool TNPCObject::Init()
{
	m_fSpeed = 150.0f;			//NPC 이동 속도
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
	m_fSpeed = 150.0f;			//NPC 이동 속도
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
}


TNPCObject::~TNPCObject()
{
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