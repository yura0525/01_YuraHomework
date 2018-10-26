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
	//MAP	-> 자물쇠 열고
	//~~	-> 상수버퍼 갱신
	//UNMAP -> 자물쇠 닫고
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

	//F1키를 누르면 와이어 프레임이 보이게함.
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
	//상수 버퍼 적용(버텍스 셰이더)
	g_pContext->VSSetConstantBuffers(0, //0 슬롯번호(레지스터 번호)
		1,								//1 상수버퍼 1개
		&m_pConstantBuffer);
	//상수버퍼 적용(픽셀 셰이더)
	g_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//알파블렌딩
	g_pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
	//텍스쳐
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

	//m_pTexture와 m_pShader는 오브젝트마다 생성이 아니라 
	//xTexManager, xShaderManager 코드에서 1개만 생성해서 필요할때 찾아준다.
	//때문에 Release해주면 안된다. 아래 주석을 풀면 안된다.
	//SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_pShader);
	return true;
}

HRESULT xObject_2D::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	//GPU상에 메모리를 할당함.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_verList.size() * sizeof(P3VERTEX);		//36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;							//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
															//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//정점 버퍼

											//GPU상에 메모리를 셋팅하고 할당함.
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

	//GPU상에 메모리를 할당함
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_indexList.size() * sizeof(DWORD);
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//인덱스 버퍼

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

	//GPU상에 메모리를 할당함
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//상수버퍼
												//GPU상에 메모리를 셋팅하고 할당함.

												//Usage
												//D3D11_USAGE_DEFAULT = 0,						 // GPU READ/WRITE가능. CPU 접근 불가능.
												//D3D11_USAGE_IMMUTABLE = 1,					 // GPU READ 가능
												//D3D11_USAGE_DYNAMIC = 2,						 // 중간에 변경가능. CPUAccessFlags 값을 셋팅해야한다.
												//D3D11_USAGE_STAGING = 3						 // CPU READ/WRITE 가능
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;					//D3D11_USAGE_DYNAMIC. GPU는 READ/WRITE 가능. CPU는 READ X /WRITE 0.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//D3D11_USAGE_DYNAMIC이면 CPUAccessFlags 값을 셋팅해야한다.
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
	//셰이더 함수의 선언. 전달인자 타입으로 POSITION을 쓰겠다.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//5번째 전달인자인 0에서부터 DXGI_FORMAT_R32G32B32_FLOAT만큼인 float 3개를 꺼낸다.
		{ "POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//5번째 전달인자인 12바이트에서부터 DXGI_FORMAT_R32G32B32A32_FLOAT float 4개를 꺼낸다.
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 },
		//5번째 전달인자인 28바이트에서 DXGI_FORMAT_R32G32_FLOAT float2개를 꺼낸다.
		{ "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	//layout.SemanticName = "POSITION";				//함수전달인자 타입.
	//layout.SemanticIndex = 0;
	//layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//POSITION은 float3개이다.
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

//알파블렌딩
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
