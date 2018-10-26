#pragma once
#include "xStd.h"
#include "xTexManager.h"
#include "xShaderManager.h"

struct D3DXVECTOR3
{
	float x, y, z;
	D3DXVECTOR3()
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	}
	D3DXVECTOR3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}
};
struct D3DXVECTOR2
{
	float u, v;
	D3DXVECTOR2()
	{
		u = 0.0f; v = 0.0f;
	}
	D3DXVECTOR2(float _u, float _v)
	{
		u = _u; v = _v;
	}
};
struct D3DXVECTOR4
{
	float r, g, b, a;
	D3DXVECTOR4()
	{
		r = 0.0f; g = 0.0f; b = 0.0f; a = 0.0f;
	}
	D3DXVECTOR4(float _r, float _g, float _b, float _a)
	{
		r = _r; g = _g; b = _b; a = _a;
	}
};
struct P3VERTEX	//pnct
{
	D3DXVECTOR3 p;		//float x, y, z;
	D3DXVECTOR4	c;		//float r, g, b, a;
	D3DXVECTOR2 t;		//float u, v;
};

//콘스턴트 버퍼, 상수버퍼. 셰이더내에서 글로벌 변수와 비슷하게 쓰인다.
//float4개 단위로 보내야한다. 안쓰더라도 float 4개단위로 보내야한다.
//fTime[0]이거 하나만 쓰더라도 float fTime[4];이렇게 선언해야한다.

//float g_fTimeX : packoffset(c1.x);			//:packoffset(c1.x);
//float g_iIndex : packoffset(c1.y);			//:packoffset(c1.y);
//float g_scale : packoffset(c1.z);			//:packoffset(c1.z);
//float g_angle : packoffset(c1.w);			//:packoffset(c1.w);
struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};

class xObject_2D
{
public:
	float						m_fScale;
	float						m_fAngle;
public:
	VS_CB						m_constantData;
	
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;

	xShader*					m_pShader;
	ID3D11InputLayout*			m_pVertexLayout;

	std::vector<P3VERTEX>		m_verList;
	std::vector<DWORD>			m_indexList;

	//texture info
	UINT						m_iTexIndex;
	xTexture*					m_pTexture;

	//알파블렌딩
	//ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	//ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	ID3D11BlendState*			m_pAlphaBlend;

	//와이어프레임
	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();

public:
	virtual HRESULT CreateVertexBuffer(ID3D11Device* pd3dDevice);
	virtual HRESULT CreateIndexBuffer(ID3D11Device* pd3dDevice);
	virtual HRESULT CreateConstantBuffer(ID3D11Device* pd3dDevice);

	virtual HRESULT CreateShader(ID3D11Device *pd3dDevice, T_STR szShaderName);		//셰이더
	virtual HRESULT CreateTexture(ID3D11Device *pd3dDevice, T_STR szTextureName);	//텍스쳐
	virtual HRESULT CreateInputLayout(ID3D11Device *pd3dDevice);

	//알파블렌딩
	virtual HRESULT SetBlendState(ID3D11Device *pd3dDevice);
	virtual HRESULT SetRasterizerState(ID3D11Device *pd3dDevice, D3D11_FILL_MODE fill = D3D11_FILL_SOLID);
public:
	xObject_2D();
	virtual ~xObject_2D();
};

