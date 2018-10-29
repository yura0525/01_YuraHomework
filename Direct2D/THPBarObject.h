#pragma once
#include "xObject.h"
class THPBarObject : public xObject
{
public:
	xObject		m_BKObject;

public:
	bool Create(ID3D11Device* pd3dDevice, float _texMaxU, float _texMaxV,
		float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
		T_STR szShaderName, T_STR szTexName, T_STR VSFunc = L"VS", T_STR PSFunc = L"PS");

	bool Frame();
	bool Render();

public:
	void DecreaseHP(int hp);

public:
	THPBarObject();
	virtual ~THPBarObject();
};

