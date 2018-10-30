#pragma once
#include "xObject.h"
class THPBarObject : public xObject
{
public:
	xObject		m_BKObject;	//HPBar 배경
	xObject*	m_pOwner;	//HPBar의 주인.

public:
	bool Create(ID3D11Device* pd3dDevice, float _texMaxU, float _texMaxV,
		float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
		T_STR szShaderName, T_STR szTexName, T_STR VSFunc = L"VS", T_STR PSFunc = L"PS");

	bool Frame();
	bool Render();
public:
	void SetOwner(xObject* pOwner)
	{
		m_pOwner = pOwner;
	}
public:
	THPBarObject(xObject* pOwner);
	virtual ~THPBarObject();
};

