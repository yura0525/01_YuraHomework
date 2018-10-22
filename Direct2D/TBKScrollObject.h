#pragma once
#include "TBKObject.h"
class TBKScrollObject : public TBKObject
{
public:
	float			m_fScrollSpeed;
	TBKObject		m_SecondBKObject;
	bool			m_IsTopBKObject;
public:
	bool Create(ID3D11Device* pd3dDevice, float _texMaxU, float _texMaxV,
		float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
		T_STR szShaderName, T_STR szTexName, T_STR VSFunc = L"VS", T_STR PSFunc = L"PS");
	bool Frame();
	bool Render();
	bool RenderScroll();
public:
	TBKScrollObject();
	virtual ~TBKScrollObject();
};

