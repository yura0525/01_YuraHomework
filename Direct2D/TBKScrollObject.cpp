#include "TBKScrollObject.h"

bool TBKScrollObject::Init()
{
	TBKObject::Init();
	m_TopObject.Init();

	return true;
}

void TBKScrollObject::SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD width, DWORD height)
{
	TBKObject::SetPosition(xPos, yPos, left, top, width, height);
	m_TopObject.SetPosition(xPos, -300, left, top, width, height);
}
void TBKScrollObject::SetTexUV(float _u, float _v)
{
	TBKObject::SetTexUV(_u, _v);
	m_TopObject.SetTexUV(_u, _v);
}

bool TBKScrollObject::Create(ID3D11Device* pd3dDevice, T_STR szShaderName, T_STR szTexName)
{
	m_TopObject.Create(pd3dDevice, szShaderName, szTexName);
	TBKObject::Create(pd3dDevice, szShaderName, szTexName);
	return true;
}

bool TBKScrollObject::Render()
{
	return RenderScroll();
}

bool TBKScrollObject::RenderScroll()
{
	//스크롤 하는 부분
	static float fStepY = 0.0f;
	fStepY += g_fSecPerFrame * m_fScrollSpeed;

	if (fStepY >= m_MaxTexUV.v)
		fStepY = 0.0f;

	float iScrollY = m_MaxTexUV.v - fStepY;

	//윗부분출력.
	
	int bottomCenter = iScrollY + ((g_rtClient.bottom - iScrollY) / 2.0f);
	//m_TopObject.SetPosition((g_rtClient.right / 2), bottomCenter, 0, iScrollY, 800, (g_rtClient.bottom - iScrollY));

	//xObject::SetPosition((g_rtClient.right / 2), (iScrollY / 2), 0, 0, 800, iScrollY);

	//Create(g_pd3dDevice, L"vertexshader.txt", L"../../data/background_00.png");
	
	
	m_TopObject.Render();
	xObject::Render();
	////두번에 나눠서 출력한다. 아래부분 출력
	//BitBlt(g_hOffScreenDC, m_rtDraw.left, fStep, m_rtDraw.right, m_rtDraw.bottom - fStep,
	//	m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);

	////윗부분 출력
	//BitBlt(g_hOffScreenDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, fStep,
	//	m_pColorBitmap->m_hMemDC, m_rtDraw.left, iScroll, SRCCOPY);
	return true;
}


TBKScrollObject::TBKScrollObject()
{
	m_fScrollSpeed = 100.0f;
}


TBKScrollObject::~TBKScrollObject()
{
}
