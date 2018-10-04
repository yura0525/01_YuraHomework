#include "xCore.h"
#include "D3Dcompiler.h"		//D3DCOMPILE_DEBUG
#include <math.h>

#define CPU
//#define GPU

struct P3VERTEX	//pnct
{
	float x,y,z;
	float r, g, b, a;
	float u,v;
};
//콘스턴트 버퍼, 상수버퍼. 셰이더내에서 글로벌 변수와 비슷하게 쓰인다.
//float4개 단위로 보내야한다. 안쓰더라도 float 4개단위로 보내야한다.
//fTime[0]이거 하나만 쓰더라도 float fTime[4];이렇게 선언해야한다.
struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};
class Sample : public xCore
{
public:
	VS_CB						m_constantData;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;

	ID3D11VertexShader*			m_pVS;
	ID3D11PixelShader*			m_pPS;
	ID3D11PixelShader*			m_pPS2;
	ID3D11PixelShader*			m_pPS3;
	ID3D11InputLayout*			m_pVertexLayout;

	std::vector<P3VERTEX>		m_verList;
	std::vector<DWORD>			m_indexList;

	//텍스쳐
	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11SamplerState*			m_pSamplerState;

	//알파블렌딩
	ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	ID3D11BlendState*			m_pAlphaBlend;
	ID3D11BlendState*			m_pAlphaBlend2;
	ID3D11BlendState*			m_pAlphaBlend3;

	//와이어프레임
	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;

public:
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();

	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();

	//텍스쳐
	HRESULT LoadTextureFile(ID3D11ShaderResourceView** ppTexSRV, const TCHAR* szFileName);

	//알파블렌딩
	HRESULT SetBlendState();

public:
	bool Init()
	{
		xCore::Init();
		
		SetRasterizerState();

		if (FAILED(SetBlendState()))
		{
			return false;
		}

		m_constantData.r = (rand() % 255) / 255.0f;
		m_constantData.g = (rand() % 255) / 255.0f;
		m_constantData.b = (rand() % 255) / 255.0f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;
		m_constantData.fTime[1] = 1.0f;
		m_constantData.fTime[2] = 1.0f;
		m_constantData.fTime[3] = 1.0f;

		LoadTextureFile(&m_pTexSRV, L"../../data/Particle4.dds");
		LoadTextureFile(&m_pTexSRVAlpha, L"../../data/hud.dds");
		LoadTextureFile(&m_pTexSRVNoAlpha, L"../../data/Dirt_Diff.dds");

		CreateVertexBuffer();
		CreateIndexBuffer();
		CreateConstantBuffer();

		if (FAILED(LoadShaderAndInputLayout()))
		{
			return false;
		}

		if (FAILED(CreatePixelShader()))
		{
			return false;
		}
		return true;
	}
	bool Frame()
	{
		xCore::Frame();

		/*static float fAngle = 0.0f;
		fAngle += g_fSecPerFrame;*/
		
		//float g_fTimeX : packoffset(c1.x);			//:packoffset(c1.x);
		//float g_iIndex : packoffset(c1.y);			//:packoffset(c1.y);
		//float g_scale : packoffset(c1.z);			//:packoffset(c1.z);
		//float g_angle : packoffset(c1.w);			//:packoffset(c1.w);
#ifdef GPU
		//gpu update
		m_constantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;
		m_constantData.fTime[1] = 0.f;
		m_constantData.fTime[2] = 1.0f;
		m_constantData.fTime[3] = 0.0f;
		m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_constantData, 0, 0);
#endif
#ifdef CPU
		//cpu update
		//MAP	-> 자물쇠 열고
		//~~	-> 상수버퍼 갱신
		//UNMAP -> 자물쇠 닫고
		D3D11_MAPPED_SUBRESOURCE address;
		HRESULT hr;
		if (SUCCEEDED(hr = m_pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
		{
			VS_CB* cb = (VS_CB*)address.pData;
			cb->r = cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->g = sinf(g_fGameTimer) * 0.5f + 0.5f;
			cb->b = cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->a = 1.0f;
			cb->fTime[0] = g_fGameTimer;
			cb->fTime[1] = 0.f;
			cb->fTime[2] = 1.0f;
			cb->fTime[3] = 0.0f;
			m_pContext->Unmap(m_pConstantBuffer, 0);
		}
#endif
		return true;
	}
	bool Render()
	{
		xCore::Render();

		m_pContext->VSSetShader(m_pVS, NULL, 0);
		m_pContext->PSSetShader(m_pPS, NULL, 0);
		m_pContext->HSSetShader(NULL, NULL, 0);
		m_pContext->DSSetShader(NULL, NULL, 0);
		m_pContext->GSSetShader(NULL, NULL, 0);
		m_pContext->IASetInputLayout(m_pVertexLayout);
		
		//W키를 누르면 와이어 프레임이 보이게함.
		if (g_Input.bFront)
		{
			m_pContext->RSSetState(m_pRSWireFrame);
		}
		else
		{
			m_pContext->RSSetState(m_pRSSolid);
		}

		UINT offset = 0;
		UINT stride = sizeof(P3VERTEX);
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//상수 버퍼 적용(버텍스 셰이더)
		m_pContext->VSSetConstantBuffers(0, //0 슬롯번호(레지스터 번호)
			1,								//1 상수버퍼 1개
			&m_pConstantBuffer);
		//상수버퍼 적용(픽셀 셰이더)
		m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//알파블렌딩
		m_pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
		//텍스쳐
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRV);
		m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);


		//알파블렌딩
		m_pContext->PSSetShader(m_pPS2, NULL, 0);
		m_pContext->OMSetBlendState(m_pAlphaBlend2, 0, -1);
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRVNoAlpha);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);

		//알파블렌딩
		m_pContext->PSSetShader(m_pPS3, NULL, 0);
		m_pContext->OMSetBlendState(m_pAlphaBlend3, 0, -1);
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRVAlpha);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);
		
		return true;
	}
	bool Release()
	{
		xCore::Release();
		
		if (m_pRSWireFrame != NULL)		m_pRSWireFrame->Release();
		if (m_pRSSolid != NULL)			m_pRSSolid->Release();

		
		if (m_pAlphaBlend != NULL)		m_pAlphaBlend->Release();
		if (m_pAlphaBlend2 != NULL)		m_pAlphaBlend2->Release();
		if (m_pAlphaBlend3 != NULL)		m_pAlphaBlend3->Release();

		if (m_pTexSRV != NULL)			m_pTexSRV->Release();
		if (m_pTexSRVNoAlpha != NULL)	m_pTexSRVNoAlpha->Release();
		if (m_pTexSRVAlpha != NULL)		m_pTexSRVAlpha->Release();

		if (m_pSamplerState != NULL)	m_pSamplerState->Release();

		if (m_pVertexBuffer != NULL)	m_pVertexBuffer->Release();
		if (m_pIndexBuffer != NULL)		m_pIndexBuffer->Release();
		if (m_pConstantBuffer != NULL)	m_pConstantBuffer->Release();

		if (m_pPS != NULL)				m_pPS->Release();
		if (m_pPS2 != NULL)				m_pPS2->Release();
		if (m_pPS3 != NULL)				m_pPS3->Release();

		if( m_pVS != NULL )				m_pVS->Release();
		if (m_pVertexLayout != NULL)	m_pVertexLayout->Release();

		m_pAlphaBlend	= m_pAlphaBlend2	= m_pAlphaBlend3	= NULL;
		m_pTexSRV		= m_pTexSRVNoAlpha	= m_pTexSRVAlpha	= NULL;

		m_pSamplerState = NULL;

		m_pVertexBuffer = NULL;
		m_pIndexBuffer = NULL;
		m_pConstantBuffer = NULL;

		m_pPS = m_pPS2 = m_pPS3 = NULL;

		m_pVS = NULL;
		m_pVertexLayout = NULL;
		return true;
	}

	HRESULT SetRasterizerState(D3D11_FILL_MODE fill = D3D11_FILL_SOLID)
	{
		HRESULT hr = S_OK;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthClipEnable = TRUE;

		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSWireFrame);

		desc.FillMode = D3D11_FILL_SOLID;
		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSSolid);
		return hr;
	}
public:
	Sample() {}
	~Sample() {}
};

HRESULT Sample::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	//정점의 저장순서 : 시계방향
	//반시계방향으로 그리면 안나온다.
	m_verList.resize(4);
	m_verList[0].x = -0.5f; m_verList[0].y = 0.5f;	m_verList[0].z = 0.5f;
	m_verList[0].r = 1.0f;	m_verList[0].g = 0.0f;	m_verList[0].b = 0.0f;
	m_verList[0].u = 0.0f; m_verList[0].v = 0.0f;

	m_verList[1].x = 0.5f;	m_verList[1].y = 0.5f;	m_verList[1].z = 0.5f;
	m_verList[1].r = 0.0f;	m_verList[1].g = 1.0f;	m_verList[1].b = 0.0f;
	m_verList[1].u = 1.0f;	m_verList[1].v = 0.0f;

	m_verList[2].x = -0.5f; m_verList[2].y = -0.5f; m_verList[2].z = 0.5f;
	m_verList[2].r = 0.0f;	m_verList[2].g = 0.0f;	m_verList[2].b = 1.0f;
	m_verList[2].u = 0.0f; m_verList[2].v = 1.0f;

	m_verList[3].x = 0.5f;  m_verList[3].y = -0.5f; m_verList[3].z = 0.5f;
	m_verList[3].r = 1.0f;  m_verList[3].g = 1.0f;	m_verList[3].b = 1.0f;
	m_verList[3].u = 1.0f;  m_verList[3].v = 1.0f;
	//vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	//vList[4].x = 0.5f;	vList[4].y = 0.5f;	vList[4].z = 0.5f;
	//vList[5].x = 0.5f;	vList[5].y = -0.5f; vList[5].z = 0.5f;

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

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &initialData, &m_pVertexBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT Sample::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
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
	int iNumCount = sizeof(m_indexList) / sizeof(m_indexList[0]);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_indexList.size() * sizeof(DWORD);
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//인덱스 버퍼
	
	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));

	InitialData.pSysMem = &(m_indexList.at(0));
	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pIndexBuffer)))
	{
		return hr;
	}

	return hr;
}
HRESULT Sample::CreateConstantBuffer()
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

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &initialData, &m_pConstantBuffer)))
	{
		return hr;
	}
	return hr;
}
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;
	
	//LoadShader
	//L"vertexshader.txt" => 셰이더파일이름, "VS" => VertexShader함수이름(), "vs_5_0"=> 컴파일러 
	ID3DBlob* pVSBuf = NULL;
	ID3DBlob* pErrorMsgs = NULL;
	DWORD dwFlags = D3DCOMPILE_DEBUG;

	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL, 
		"VS", "vs_5_0", dwFlags, NULL, NULL, &pVSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return hr;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	//InputLayout.
	//셰이더 함수의 선언. 전달인자 타입으로 POSITION을 쓰겠다.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//5번째 전달인자인 0에서부터 DXGI_FORMAT_R32G32B32_FLOAT만큼인 float 3개를 꺼낸다.
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//5번째 전달인자인 12바이트에서부터 DXGI_FORMAT_R32G32B32A32_FLOAT float 4개를 꺼낸다.
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 },
		//5번째 전달인자인 28바이트에서 DXGI_FORMAT_R32G32_FLOAT float2개를 꺼낸다.
		{ "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 28,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	m_pd3dDevice->CreateInputLayout(layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);
	pVSBuf->Release();

	return hr;
}
HRESULT Sample::CreatePixelShader()
{
	HRESULT hr = S_OK;
	
	//LoadShader
	//L"vertexshader.txt" => 셰이더파일이름, "PS" => PixelShader함수이름(), "ps_5_0"=> 컴파일러 
	ID3DBlob* pPSBuf = NULL;
	ID3DBlob* pErrorMsgs = NULL;
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"PS", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return hr;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();


	//2)
	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"PS2", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return hr;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS2));
	pPSBuf->Release();

	//3)
	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"PS3", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return hr;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS3));
	pPSBuf->Release();
	return hr;
}

HRESULT Sample::LoadTextureFile(ID3D11ShaderResourceView** ppTexSRV, const TCHAR* szFileName)
{
	HRESULT hr = S_OK;

	//D3DX11_IMAGE_LOAD_INFO	pLoadInfo;
	//D3DX11_IMAGE_INFO		pSrcInfo;

	//ZeroMemory(&pLoadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//pLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
	//pLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//pLoadInfo.Width = D3DX11_DEFAULT;
	//pLoadInfo.Height = D3DX11_DEFAULT;
	//pLoadInfo.pSrcInfo = &pSrcInfo;
	//pLoadInfo.MipLevels = 1;

	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szFileName, NULL, NULL, ppTexSRV, NULL)))
	{
		return hr;
	}

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter	= D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	m_pd3dDevice->CreateSamplerState(&sd, &m_pSamplerState);
	return hr;
}

HRESULT Sample::SetBlendState()
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
	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState**)&m_pAlphaBlend);

	//2)
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState**)&m_pAlphaBlend2);

	//3)
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState**)&m_pAlphaBlend3);
	return hr;
}
GAMERUN("CreateDevice", 800, 600);