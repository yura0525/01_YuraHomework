#pragma once
#include "xStd.h"

class xShader
{
public:
	T_STR m_szName;

public:
	ID3D11VertexShader *		m_pVS;
	ID3D11PixelShader*			m_pPS;
	ID3DBlob*					m_pBlobVS;
	ID3DBlob*					m_pBlobPS;
public:
	ID3D11Device *				m_pd3dDevice;		//디바이스 객체
	ID3D11ShaderResourceView*	m_pTexSRV;

public:
	bool Load(ID3D11Device* pd3dDevice, T_STR szFileName, C_STR vs = "VS", C_STR ps = "PS");

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	xShader();
	virtual ~xShader();
};

