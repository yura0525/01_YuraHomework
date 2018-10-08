#pragma once
#include "xStd.h"

class xTexture
{
public:
	T_STR m_szName;

public:
	ID3D11Device * m_pd3dDevice;		//디바이스 객체

	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11SamplerState*			m_pSamplerState;

public:
	bool Load(ID3D11Device* pd3dDevice, T_STR szFileName);

public:
	bool Init();			//초기화
	bool Frame();			//계산
	bool Render();			//드로우
	bool Release();			//소멸, 삭제

public:
	xTexture();
	virtual ~xTexture();
};

