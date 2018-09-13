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
//�ܽ���Ʈ ����, �������. ���̴������� �۷ι� ������ ����ϰ� ���δ�.
//float4�� ������ �������Ѵ�. �Ⱦ����� float 4�������� �������Ѵ�.
//fTime[0]�̰� �ϳ��� ������ float fTime[4];�̷��� �����ؾ��Ѵ�.
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
		//MAP	-> �ڹ��� ����
		//~~	-> ������� ����
		//UNMAP -> �ڹ��� �ݰ�
		
		//�ۼ����̴�!!!!!!

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

	//������ ������� : �ð����
	//�ݽð�������� �׸��� �ȳ��´�.
	vList[0].x = -0.5f; vList[0].y = 0.5f;	vList[0].z = 0.5f;
	vList[1].x = 0.5f;	vList[1].y = 0.5f;	vList[1].z = 0.5f;
	vList[2].x = -0.5f; vList[2].y = -0.5f; vList[2].z = 0.5f;
	vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f;	vList[4].y = 0.5f;	vList[4].z = 0.5f;
	vList[5].x = 0.5f;	vList[5].y = -0.5f; vList[5].z = 0.5f;

	//GPU�� �޸𸮸� �Ҵ���.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = 6 * sizeof(P3VERTEX);	//36����Ʈ
	bd.Usage = D3D11_USAGE_DEFAULT;			//GPU�� �޸𸮸� �Ҵ��ض�. �⺻�� GPU�޸�. GPU�� READ/WRITE ����.CPU�� ���ٺҰ����ϴ�.
											//D3D11_USAGE_STAGING���� CPU�� ���ٰ����ϴ�. ������ ������.

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���� ����

	//GPU�� �޸𸮸� �����ϰ� �Ҵ���.
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
	//L"vertexshader.txt" => ���̴������̸�, "VS" => VertexShader�Լ��̸�(), "vs_5_0"=> �����Ϸ� 
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pVSBuf, NULL, NULL));

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	//InputLayout.
	//���̴� �Լ��� ����. �������� Ÿ������ POSITION�� ���ڴ�.
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
	//L"vertexshader.txt" => ���̴������̸�, "PS" => PixelShader�Լ��̸�(), "ps_5_0"=> �����Ϸ� 
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pPSBuf, NULL, NULL));

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();
	return hr;
}

GAMERUN("CreateDevice", 800, 600);