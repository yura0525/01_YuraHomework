#pragma once
#include "xStd.h"
#include "xTexManager.h"
#include "xShaderManager.h"

struct P3VERTEX	//pnct
{
	float x, y, z;
	float r, g, b, a;
	float u, v;
};
//콘스턴트 버퍼, 상수버퍼. 셰이더내에서 글로벌 변수와 비슷하게 쓰인다.
//float4개 단위로 보내야한다. 안쓰더라도 float 4개단위로 보내야한다.
//fTime[0]이거 하나만 쓰더라도 float fTime[4];이렇게 선언해야한다.
struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};

class xObject
{
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
	ID3DBlob*					m_pBlobVS;

	//알파블렌딩
	//ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	//ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	ID3D11BlendState*			m_pAlphaBlend;

	//와이어프레임
	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;

public:
	virtual bool Init();
	virtual bool Frame(ID3D11DeviceContext* pContext);
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();
	virtual bool Create(ID3D11Device* pd3dDevice, T_STR szShaderName, T_STR szTexName);

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
	xObject();
	virtual ~xObject();
};

