#include "xCore.h"
#include "D3Dcompiler.h"		//D3DCOMPILE_DEBUG
#include <math.h>

#define CPU
//#define GPU

struct P3VERTEX
{
	float x;
	float y;
	float z;
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
	VS_CB					m_constantData;

	ID3D11Buffer *			m_pVertexBuffer;
	ID3D11Buffer *			m_pIndexBuffer;
	ID3D11Buffer *			m_pConstantBuffer;

	ID3D11VertexShader*		m_pVS;
	ID3D11PixelShader*		m_pPS;
	ID3D11InputLayout*		m_pVertexLayout;

	std::vector<P3VERTEX>	m_verList;
	std::vector<DWORD>		m_indexList;

public:
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();

	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();

public:
	bool Init()
	{
		xCore::Init();
		
		m_constantData.r = cosf(g_fGameTimer) * 0.5 + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5 + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;

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
		
		static float fAngle = 0.0f;
		fAngle += g_fSecPerFrame;
		//gpu update
#ifdef GPU
		m_constantData.r = cosf(g_fGameTimer) * 0.5 + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5 + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;
		m_constantData.fTime[1] = 0.5f;
		m_constantData.fTime[2] = 1.0f;
		m_constantData.fTime[3] = fAngle;
		m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_constantData, 0, 0);
#elif define CPU
		//cpu update
		//MAP	-> 자물쇠 열고
		//~~	-> 상수버퍼 갱신
		//UNMAP -> 자물쇠 닫고
		
		//작성중이다!!!!!!

#endif
		return true;
	}
	bool Render()
	{
		xCore::Render();

		UINT offset = 0;
		UINT stride = sizeof(P3VERTEX);
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->IASetInputLayout(m_pVertexLayout);
		m_pContext->VSSetShader(m_pVS, NULL, 0);
		m_pContext->HSSetShader(NULL, NULL, 0);
		m_pContext->DSSetShader(NULL, NULL, 0);
		m_pContext->GSSetShader(NULL, NULL, 0);
		m_pContext->PSSetShader(m_pPS, NULL, 0);

		m_pContext->Draw(6, 0);

		return true;
	}
	bool Release()
	{
		xCore::Release();
		
		if (m_pPS != NULL) m_pPS->Release();
		if (m_pVertexLayout != NULL) m_pVertexLayout->Release();
		if( m_pVS != NULL ) m_pVS->Release();
		
		m_pPS = NULL;
		m_pVertexLayout = NULL;
		m_pVS = NULL;
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

HRESULT Sample::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	P3VERTEX vList[6];

	//정점의 저장순서 : 시계방향
	//반시계방향으로 그리면 안나온다.
	vList[0].x = -0.5f; vList[0].y = 0.5f;	vList[0].z = 0.5f;
	vList[1].x = 0.5f;	vList[1].y = 0.5f;	vList[1].z = 0.5f;
	vList[2].x = -0.5f; vList[2].y = -0.5f; vList[2].z = 0.5f;
	vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f;	vList[4].y = 0.5f;	vList[4].z = 0.5f;
	vList[5].x = 0.5f;	vList[5].y = -0.5f; vList[5].z = 0.5f;

	//GPU상에 메모리를 할당함.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = 6 * sizeof(P3VERTEX);	//36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;			//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
											//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//정점 버퍼

	//GPU상에 메모리를 셋팅하고 할당함.
	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = vList;

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &initialData, &m_pVertexBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT Sample::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT Sample::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBuf = NULL;

	//LoadShader
	//L"vertexshader.txt" => 셰이더파일이름, "VS" => VertexShader함수이름(), "vs_5_0"=> 컴파일러 
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pVSBuf, NULL, NULL));

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	//InputLayout.
	//셰이더 함수의 선언. 전달인자 타입으로 POSITION을 쓰겠다.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	m_pd3dDevice->CreateInputLayout(layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);
	pVSBuf->Release();

	return hr;
}
HRESULT Sample::CreatePixelShader()
{
	HRESULT hr = S_OK;
	ID3DBlob* pPSBuf = NULL;
	//LoadShader
	//L"vertexshader.txt" => 셰이더파일이름, "PS" => PixelShader함수이름(), "ps_5_0"=> 컴파일러 
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pPSBuf, NULL, NULL));

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();
	return hr;
}

GAMERUN("CreateDevice", 800, 600);