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

//�ܽ���Ʈ ����, �������. ���̴������� �۷ι� ������ ����ϰ� ���δ�.
//float4�� ������ �������Ѵ�. �Ⱦ����� float 4�������� �������Ѵ�.
//fTime[0]�̰� �ϳ��� ������ float fTime[4];�̷��� �����ؾ��Ѵ�.
struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};

class xObject_2D
{
public:
	VS_CB						m_constantData;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;

	xShader*					m_pShader;
	ID3D11InputLayout*			m_pVertexLayout;

	P3VERTEX					m_verList[4];
	DWORD						m_indexList[6];

	//texture info
	UINT						m_iTexIndex;
	xTexture*					m_pTexture;

	//���ĺ���
	//ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	//ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	ID3D11BlendState*			m_pAlphaBlend;

	//���̾�������
	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
	virtual bool Create(ID3D11Device* pd3dDevice, T_STR szShaderName, T_STR szTexName);

public:
	virtual HRESULT CreateVertexBuffer(ID3D11Device* pd3dDevice);
	virtual HRESULT CreateIndexBuffer(ID3D11Device* pd3dDevice);
	virtual HRESULT CreateConstantBuffer(ID3D11Device* pd3dDevice);

	virtual HRESULT CreateShader(ID3D11Device *pd3dDevice, T_STR szShaderName);		//���̴�
	virtual HRESULT CreateTexture(ID3D11Device *pd3dDevice, T_STR szTextureName);	//�ؽ���
	virtual HRESULT CreateInputLayout(ID3D11Device *pd3dDevice);

	//���ĺ���
	virtual HRESULT SetBlendState(ID3D11Device *pd3dDevice);
	virtual HRESULT SetRasterizerState(ID3D11Device *pd3dDevice, D3D11_FILL_MODE fill = D3D11_FILL_SOLID);
public:
	xObject_2D();
	virtual ~xObject_2D();
};

