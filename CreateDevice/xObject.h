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
//�ܽ���Ʈ ����, �������. ���̴������� �۷ι� ������ ����ϰ� ���δ�.
//float4�� ������ �������Ѵ�. �Ⱦ����� float 4�������� �������Ѵ�.
//fTime[0]�̰� �ϳ��� ������ float fTime[4];�̷��� �����ؾ��Ѵ�.
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

	//���ĺ���
	//ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	//ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	ID3D11BlendState*			m_pAlphaBlend;

	//���̾�������
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

	virtual HRESULT CreateShader(ID3D11Device *pd3dDevice, T_STR szShaderName);		//���̴�
	virtual HRESULT CreateTexture(ID3D11Device *pd3dDevice, T_STR szTextureName);	//�ؽ���
	virtual HRESULT CreateInputLayout(ID3D11Device *pd3dDevice);

	//���ĺ���
	virtual HRESULT SetBlendState(ID3D11Device *pd3dDevice);
	virtual HRESULT SetRasterizerState(ID3D11Device *pd3dDevice, D3D11_FILL_MODE fill = D3D11_FILL_SOLID);
public:
	xObject();
	virtual ~xObject();
};

