#include "xCore.h"

struct P3VERTEX
{
	float x;
	float y;
	float z;
};

class Sample : public xCore
{
public:
	ID3D11Buffer *			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVS;
	ID3D11PixelShader*		m_pPS;
	ID3D11InputLayout*		m_pVertexLayout;

public:
	HRESULT CreateVertexBuffer();
	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();

public:
	bool Init()
	{
		xCore::Init();
		
		CreateVertexBuffer();

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
		
		TCHAR	m_csBuffer[256];

		/*if (g_Input.bFront)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bFront!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bFront)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bFront!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bBack)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bBack!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bLeft)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bLeft!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bRight)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bRight!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bAttack)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bAttack!!!!!\n");
			OutputDebugString(m_csBuffer);
		}
		if (g_Input.bJump)
		{
			_stprintf_s(m_csBuffer, L"g_Input.bJump!!!!!\n");
			OutputDebugString(m_csBuffer);
		}*/

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
	vList[0].x = -0.5f; vList[0].y = 0.5f; vList[0].z = 0.5f;
	vList[1].x = 0.5f; vList[1].y = 0.5f; vList[1].z = 0.5f;
	vList[2].x = -0.5f; vList[2].y = -0.5f; vList[2].z = 0.5f;
	vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f; vList[4].y = 0.5f; vList[4].z = 0.5f;
	vList[5].x = 0.5f; vList[5].y = -0.5f; vList[5].z = 0.5f;

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
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBuf = NULL;

	//LoadShader
	//L"vertexshader.txt" => ���̴������̸�, "VS" => VertexShader�Լ��̸�(), "vs_5_0"=> �����Ϸ� 
	V_RETURN(D3DX11CompilerFromFile(L"vertexshader.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pVSBuf, NULL, NULL));

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	//InputLayout
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
	V_RETURN(D3DX11CompilerFromFile(L"vertexshader.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pPSBuf, NULL, NULL));

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();
	return hr;
}

GAMERUN("CreateDevice", 800, 600);