#include "xObject_2D.h"
#include "xInput.h"

#define GPU
bool xObject_2D::Init()
{
	m_constantData.r = 0.0f;
	m_constantData.g = 0.0f;
	m_constantData.b = 0.0f;
	m_constantData.a = 1;
	m_constantData.fTime[0] = g_fGameTimer;
	m_constantData.fTime[1] = 1.0f;
	m_constantData.fTime[2] = 1.0f;
	m_constantData.fTime[3] = 1.0f;

	return true;
}
bool xObject_2D::Frame()
{
	if (g_pContext == NULL)
		return false;

	static float fAngle = 0.0f;
	fAngle += g_fSecPerFrame;

	ID3D11DeviceContext* pContext = g_pContext;
	
#ifdef GPU
	//gpu update
	m_constantData.r = 0.0f;
	m_constantData.g = 0.0f;
	m_constantData.b = 0.0f;
	m_constantData.a = 1;
	m_constantData.fTime[0] = g_fGameTimer;
	m_constantData.fTime[1] = 0.0f;
	m_constantData.fTime[2] = m_fScale;
	m_constantData.fTime[3] = m_fAngle;
	//m_constantData.fTime[3] = fAngle;
	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_verList[0], 0, 0);
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_constantData, 0, 0);
#endif
#ifdef CPU
	//cpu update
	//MAP	-> �ڹ��� ����
	//~~	-> ������� ����
	//UNMAP -> �ڹ��� �ݰ�
	D3D11_MAPPED_SUBRESOURCE address;
	HRESULT hr;
	if (SUCCEEDED(hr = pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
	{
		VS_CB* cb = (VS_CB*)address.pData;
		cb->r = 0.0f;
		cb->g = 0.0f;
		cb->b = 0.0f;
		cb->a = 1.0f;
		cb->fTime[0] = g_fGameTimer;
		cb->fTime[1] = 0.0f;
		cb->fTime[2] = m_fScale;
		cb->fTime[3] = 1.0f;
		//cb->fTime[3] = fAngle;
		pContext->Unmap(m_pConstantBuffer, 0);
	}
#endif

	return true;
}
bool xObject_2D::PreRender()
{
	if (g_pContext == NULL)
		return false;

	g_pContext->VSSetShader(m_pShader->m_pVS, NULL, 0);
	g_pContext->PSSetShader(m_pShader->m_pPS, NULL, 0);
	g_pContext->HSSetShader(NULL, NULL, 0);
	g_pContext->DSSetShader(NULL, NULL, 0);
	g_pContext->GSSetShader(NULL, NULL, 0);
	g_pContext->IASetInputLayout(m_pVertexLayout);

	//F1Ű�� ������ ���̾� �������� ���̰���.
	if (I_Input.m_KeyState[DIK_F1])
		g_pContext->RSSetState(m_pRSWireFrame);
	else
		g_pContext->RSSetState(m_pRSSolid);

	return true;
}
bool xObject_2D::Render()
{
	
	PreRender();

	UINT offset = 0;
	UINT stride = sizeof(P3VERTEX);
	g_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//��� ���� ����(���ؽ� ���̴�)
	g_pContext->VSSetConstantBuffers(0, //0 ���Թ�ȣ(�������� ��ȣ)
		1,								//1 ������� 1��
		&m_pConstantBuffer);
	//������� ����(�ȼ� ���̴�)
	g_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���ĺ���
	g_pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
	//�ؽ���
	g_pContext->PSSetShaderResources(0, 1, &(m_pTexture->m_pTexSRV));
	g_pContext->PSSetSamplers(0, 1, &(m_pTexture->m_pSamplerState));
	
	PostRender();
	return true;
}
bool xObject_2D::PostRender()
{
	g_pContext->DrawIndexed(m_indexList.size(), 0, 0);
	return true;
}
bool xObject_2D::Release()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pRSWireFrame);
	SAFE_RELEASE(m_pRSSolid);

	//m_pTexture�� m_pShader�� ������Ʈ���� ������ �ƴ϶� 
	//xTexManager, xShaderManager �ڵ忡�� 1���� �����ؼ� �ʿ��Ҷ� ã���ش�.
	//������ Release���ָ� �ȵȴ�. �Ʒ� �ּ��� Ǯ�� �ȵȴ�.
	//SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_pShader);
	return true;
}

HRESULT xObject_2D::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	//GPU�� �޸𸮸� �Ҵ���.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_verList.size() * sizeof(P3VERTEX);		//36����Ʈ
	bd.Usage = D3D11_USAGE_DEFAULT;							//GPU�� �޸𸮸� �Ҵ��ض�. �⺻�� GPU�޸�. GPU�� READ/WRITE ����.CPU�� ���ٺҰ����ϴ�.
															//D3D11_USAGE_STAGING���� CPU�� ���ٰ����ϴ�. ������ ������.

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���� ����

											//GPU�� �޸𸮸� �����ϰ� �Ҵ���.
	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = &(m_verList.at(0));

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &initialData, &m_pVertexBuffer)))
	{
		return hr;
	}
	return hr;
}
HRESULT xObject_2D::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	m_indexList.clear();
	m_indexList.push_back(0);
	m_indexList.push_back(1);
	m_indexList.push_back(2);
	m_indexList.push_back(2);
	m_indexList.push_back(1);
	m_indexList.push_back(3);

	/*DWORD indices[] =
	{
	0,1,2,
	2,1,3,
	};*/

	//GPU�� �޸𸮸� �Ҵ���
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_indexList.size() * sizeof(DWORD);
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU�� �޸𸮸� �Ҵ��ض�. �⺻�� GPU�޸�. GPU�� READ/WRITE ����.CPU�� ���ٺҰ����ϴ�.
												//D3D11_USAGE_STAGING���� CPU�� ���ٰ����ϴ�. ������ ������.
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//�ε��� ����

	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));

	InitialData.pSysMem = &(m_indexList.at(0));
	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pIndexBuffer)))
	{
		return hr;
	}

	return hr;
}

HRESULT xObject_2D::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	//GPU�� �޸𸮸� �Ҵ���
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�������
												//GPU�� �޸𸮸� �����ϰ� �Ҵ���.

												//Usage
												//D3D11_USAGE_DEFAULT = 0,						 // GPU READ/WRITE����. CPU ���� �Ұ���.
												//D3D11_USAGE_IMMUTABLE = 1,					 // GPU READ ����
												//D3D11_USAGE_DYNAMIC = 2,						 // �߰��� ���氡��. CPUAccessFlags ���� �����ؾ��Ѵ�.
												//D3D11_USAGE_STAGING = 3						 // CPU READ/WRITE ����
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;					//D3D11_USAGE_DYNAMIC. GPU�� READ/WRITE ����. CPU�� READ X /WRITE 0.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//D3D11_USAGE_DYNAMIC�̸� CPUAccessFlags ���� �����ؾ��Ѵ�.
#endif
#ifdef GPU
	bd.Usage = D3D11_USAGE_DEFAULT;
#endif

	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = &(m_constantData);

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &initialData, &m_pConstantBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT xObject_2D::CreateShader(ID3D11Device *pd3dDevice, T_STR szShaderName)
{
	int iIndex = I_ShaderMgr.Load(pd3dDevice, szShaderName);
	m_pShader = I_ShaderMgr.GetPtr(iIndex);
	return S_OK;
}

HRESULT xObject_2D::CreateTexture(ID3D11Device *pd3dDevice, T_STR szTextureName)
{
	int iIndex = I_TextureMgr.Load(pd3dDevice, szTextureName);
	m_pTexture = I_TextureMgr.GetPtr(iIndex);
	return S_OK;
}

HRESULT xObject_2D::CreateInputLayout(ID3D11Device *pd3dDevice)
{
	//InputLayout.
	//���̴� �Լ��� ����. �������� Ÿ������ POSITION�� ���ڴ�.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//5��° ���������� 0�������� DXGI_FORMAT_R32G32B32_FLOAT��ŭ�� float 3���� ������.
		{ "POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//5��° ���������� 12����Ʈ�������� DXGI_FORMAT_R32G32B32A32_FLOAT float 4���� ������.
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 },
		//5��° ���������� 28����Ʈ���� DXGI_FORMAT_R32G32_FLOAT float2���� ������.
		{ "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	//layout.SemanticName = "POSITION";				//�Լ��������� Ÿ��.
	//layout.SemanticIndex = 0;
	//layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//POSITION�� float3���̴�.
	//layout.InputSlot = 0;
	//layout.AlignedByteOffset = 0;
	//layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//layout.InstanceDataStepRate = 0;
	ID3DBlob* pBlobVS = m_pShader->m_pBlobVS;
	if (m_pShader == NULL || pBlobVS == NULL)
		return false;

	pd3dDevice->CreateInputLayout(layout, iNum, pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &m_pVertexLayout);
	return S_OK;
}

//���ĺ���
HRESULT xObject_2D::SetBlendState(ID3D11Device *pd3dDevice)
{
	HRESULT hr = S_OK;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

	//FinalColor = destColor * DescBlend + srcColor * SrcBlend;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState**)&m_pAlphaBlend);
	return hr;
}

HRESULT xObject_2D::SetRasterizerState(ID3D11Device *pd3dDevice, D3D11_FILL_MODE fill)
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;

	hr = pd3dDevice->CreateRasterizerState(&desc, &m_pRSWireFrame);

	desc.FillMode = D3D11_FILL_SOLID;
	hr = pd3dDevice->CreateRasterizerState(&desc, &m_pRSSolid);
	return hr;
}
xObject_2D::xObject_2D()
{
	m_fScale = 1.0f;
	m_fAngle = 0.0f;
}


xObject_2D::~xObject_2D()
{
}
