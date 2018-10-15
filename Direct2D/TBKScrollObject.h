#pragma once
#include "TBKObject.h"
class TBKScrollObject : public TBKObject
{
public:
	float			m_fScrollSpeed;
	TBKObject		m_BottomObject;
public:

	bool Init();
	void SetTexUV(float _u, float _v);
	bool Create(ID3D11Device* pd3dDevice, T_STR szShaderName, T_STR szTexName);

	bool Render();
	bool RenderScroll();
public:
	TBKScrollObject();
	virtual ~TBKScrollObject();
};

