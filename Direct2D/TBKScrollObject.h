#pragma once
#include "TBKObject.h"
class TBKScrollObject : public TBKObject
{
public:
	float			m_fScrollSpeed;
	TBKObject		m_TopObject;
public:

	bool Init();
	void SetTexUV(float _u, float _v);
	void SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD width, DWORD height);
	bool Create(ID3D11Device* pd3dDevice, T_STR szShaderName, T_STR szTexName);

	bool Render();
	bool RenderScroll();
public:
	TBKScrollObject();
	virtual ~TBKScrollObject();
};

