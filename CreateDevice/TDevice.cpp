#include "TDevice.h"

HRESULT TDevice::CreateDevice()
{
	HRESULT hr = S_OK;

	//1)디바이스 생성
	UINT Flags = 0;
	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL pFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	//pFeatureLevels의 갯수이다.
	UINT FeatureLevels = 2;

	//하드웨어에서 지원하는것이 그래픽카드가 지원하는것
	D3D_DRIVER_TYPE DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	//GPU가 연산
		D3D_DRIVER_TYPE_WARP,		//CPU가 연산
		D3D_DRIVER_TYPE_REFERENCE,	//CPU가 연산
	};

	UINT numDriverType = sizeof(DriverType) / sizeof(D3D_DRIVER_TYPE);

	//ALT + ENTER키를 막는다.
	//IDXGIAdapter* pAdapter = NULL;
	//m_pDXGIFactory->EnumAdapters(0, &pAdapter);

	for (int iMode = 0; iMode < numDriverType; iMode++)
	{
		D3D_FEATURE_LEVEL retFL;

		//ALT + ENTER키를 막는다.
		//if (SUCCEEDED(D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, Flags,
		//	pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &retFL, &m_pContext)))
		if (SUCCEEDED(D3D11CreateDevice(NULL, DriverType[iMode], NULL, Flags,
			pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &retFL, &m_pContext)))
		{
			if (retFL != D3D_FEATURE_LEVEL_11_0)
			{
				Release();
				continue;
			}
			break;
		}
	}
	return true;
}

HRESULT TDevice::CreateDX11GIFactory()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory)))
	{
		return false;
	}

	return true;
}

HRESULT TDevice::CreateSwapChain()
{
	//백버퍼를 만드는것.
	ZeroMemory(&m_sd, sizeof(DXGI_SWAP_CHAIN_DESC));

	m_sd.BufferDesc.Width = g_rtClient.right;
	m_sd.BufferDesc.Height = g_rtClient.bottom;

	m_sd.BufferDesc.RefreshRate.Numerator = 60;
	m_sd.BufferDesc.RefreshRate.Denominator = 1;			//60메가 헤르츠 1/60
	m_sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//백버퍼의 용도.
	m_sd.BufferCount = 1;									//백버퍼의 갯수.
	m_sd.OutputWindow = g_hWnd;
	m_sd.Windowed = true;
	m_sd.SampleDesc.Count = 1;							//한번만 뿌린다. 여러번 뿌리면 안티앨리어싱.


	//초기해상도 대로 띄운다. 전체 화면 전환시에 자동으로 바탕화면 해상도로 변경되는걸 막는다.
	m_sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	HRESULT hr = S_OK;
	if (FAILED(hr = m_pDXGIFactory->CreateSwapChain(m_pd3dDevice, &m_sd, &m_pSwapChain)))
	{
		return hr;
	}
	return hr;
}

HRESULT TDevice::SetRenderTarget()
{
	HRESULT hr = S_OK;
	//2)랜더타겟 뷰 생성
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	//생성과 관련된 메소드는 m_pd3dDevice와 관련
	//생성된 이후의 제어는 m_pd3dDeviceContext와 관련
	if (FAILED(hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView)))
	{
		return hr;
	}

	//GetBuffer을 쓰고 원하는 작업을 한뒤에는 무조건 삭제를 해야한다. Release();
	pBackBuffer->Release();
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	return hr;
}

void TDevice::SetViewPort()
{
	//3)뷰포트 생성
	//화면 전체를 채워라.
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = g_rtClient.right;
	vp.Height = g_rtClient.bottom;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &vp);	//Rasterizer Stage에 셋팅.
}
HRESULT TDevice::CreateDeviceResources(UINT iWidth, UINT iHeight)
{
	return S_OK;
}

void TDevice::DeleteDeviceResources(UINT iWidth, UINT iHeight)
{

}
//백버퍼는 화면크기에 맞춰서 생성되었으므로, 화면크기가 변하면 백버퍼를 다시 생성해야한다.
HRESULT TDevice::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == NULL)	return -1;

	HRESULT hr = S_OK;

	//랜더타겟을 널로 셋팅하고 해제.
	m_pContext->OMSetRenderTargets(0, NULL, NULL);
	DeleteDeviceResources(iWidth, iHeight);

	if (m_pRenderTargetView != NULL) m_pRenderTargetView->Release();

	//백버퍼의 크기를 조정한다.
	m_pSwapChain->ResizeBuffers(m_sd.BufferCount, m_sd.BufferDesc.Width, m_sd.BufferDesc.Height, 
		m_sd.BufferDesc.Format, m_sd.Flags);

	//랜더타겟뷰 생성및 적용한다.
	if (FAILED(hr = SetRenderTarget()))
	{
		return hr;
	}
	SetViewPort();

	CreateDeviceResources(iWidth, iHeight);
	return hr;
}

bool TDevice::Init()
{
	HRESULT hr;
	if (FAILED(hr = CreateDX11GIFactory()))
	{
		return false;
	}

	if (FAILED(hr = CreateDevice()))
	{
		return false;
	}

	if (FAILED(hr = CreateSwapChain()))
	{
		return false;
	}

	if (FAILED(hr = SetRenderTarget()))
	{
		return false;
	}

	SetViewPort();

	//ALT + ENTER키를 막는다.
	if (FAILED(hr = m_pDXGIFactory->MakeWindowAssociation(NULL, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return hr;
	}
	return true;
}


bool TDevice::PreRender()
{
	//RGB 255가 0~1의 값으로 컨버팅. RGBA
	float color[4] = { 1,1,0,1 };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);
	return true;
}

bool TDevice::PostRender()
{
	//백버퍼에 있는걸 전면버퍼로 바꾼다.
	m_pSwapChain->Present(0, 0);
	return true;
}

bool TDevice::Render()
{
	////지형, 캐릭터, 이펙트, UI
	return true;
}

bool TDevice::Release()
{
	//장치의 모든 기본 설정을 복원하고 장치를 만들때 상태로 되돌린다.
	if (m_pContext != NULL)	m_pContext->ClearState();

	//생성한 역순으로 Release한다.
	if (m_pRenderTargetView != NULL)		m_pRenderTargetView->Release();
	if (m_pSwapChain != NULL)				m_pSwapChain->Release();
	if (m_pContext != NULL)					m_pContext->Release();
	if (m_pd3dDevice != NULL)				m_pd3dDevice->Release();

	m_pRenderTargetView = NULL;
	m_pSwapChain = NULL;
	m_pContext = NULL;
	m_pd3dDevice = NULL;
	return true;
}


TDevice::TDevice()
{
}


TDevice::~TDevice()
{
}
