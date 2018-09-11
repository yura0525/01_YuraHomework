#pragma once
#include "xStd.h"

class TDevice
{
public:
	ID3D11Device * m_pd3dDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGIFactory*			m_pDXGIFactory;
	IDXGISwapChain*			m_pSwapChain;			//WINAPI의 더블버퍼링이랑 같음. 백버퍼를 만든다.
	ID3D11RenderTargetView*	m_pRenderTargetView;

	DXGI_SWAP_CHAIN_DESC	m_sd;
public:
	HRESULT CreateDevice();
	HRESULT CreateDX11GIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTarget();
	void	SetViewPort();

public:
	void CreateDeviceResource(UINT iWidth, UINT iHeight);
	void DeleteDeviceResource(UINT iWidth, UINT iHeight);
	HRESULT ResizeDevice(UINT iWidth, UINT iHeight);
public:
	bool Init();
	virtual bool Frame()
	{
		return true;
	}
	bool PreRender();
	bool PostRender();
	bool Render();
	bool Release();

public:
	TDevice();
	virtual ~TDevice();
};

