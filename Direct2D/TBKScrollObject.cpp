#include "TBKScrollObject.h"

bool TBKScrollObject::Create(ID3D11Device* pd3dDevice, float texMaxU, float texMaxV, float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
	T_STR szShaderName, T_STR szTexName, T_STR VSFunc, T_STR PSFunc)
{
	//처음엔 m_SecondBKObject이 TOPObject이다.
	m_SecondBKObject.Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos - g_rtClient.bottom, left, top, right, bottom, szShaderName, szTexName, VSFunc, PSFunc);
	TBKObject::Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos, left, top, right, bottom, szShaderName, szTexName, VSFunc, PSFunc);
	return true;
}

bool  TBKScrollObject::Frame()
{
	//스크롤 하는 부분
	static float fStepY = 0.0f;
	static float fStepYSum = 0.0f;
	fStepY = g_fSecPerFrame * m_fScrollSpeed;
	fStepYSum += fStepY;

	m_SecondBKObject.m_pos.y += fStepY;
	m_pos.y += fStepY;

	//스크롤이 현재오브젝트의 탑까지 오면 아래에 있는걸 다시 위로 올린다.
	//번갈아 가면서 올린다.
	//노란색 경계가 생겨서 -10을 빼서 미리 올려주었다.
	if (fStepYSum >= (g_rtClient.bottom -10))
	{
		int topPosX = (g_rtClient.right / 2);
		int topPosY = (g_rtClient.bottom / 2) - g_rtClient.bottom;
		if (m_IsTopBKObject == false)
		{
			TBKObject::SetPosition(topPosX, topPosY);
		}
		else
		{
			m_SecondBKObject.SetPosition(topPosX, topPosY);
		}
		fStepYSum = 0.0f;
		m_IsTopBKObject = !m_IsTopBKObject;
	}

	m_SecondBKObject.Frame();
	return TBKObject::Frame();
}

bool TBKScrollObject::Render()
{
	return RenderScroll();
}

bool TBKScrollObject::RenderScroll()
{
	m_SecondBKObject.Render();
	return xObject::Render();
}


TBKScrollObject::TBKScrollObject()
{
	m_fScrollSpeed = 100.0f;
	m_IsTopBKObject = false;
}


TBKScrollObject::~TBKScrollObject()
{
}
