#include "xTexture.h"

bool xTexture::Load(ID3D11Device* pd3dDevice, T_STR szFileName)
{
	//D3DX11_IMAGE_LOAD_INFO	pLoadInfo;
	//D3DX11_IMAGE_INFO		pSrcInfo;

	//ZeroMemory(&pLoadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//pLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
	//pLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//pLoadInfo.Width = D3DX11_DEFAULT;
	//pLoadInfo.Height = D3DX11_DEFAULT;
	//pLoadInfo.pSrcInfo = &pSrcInfo;
	//pLoadInfo.MipLevels = 1;

	m_pd3dDevice = pd3dDevice;
	HRESULT hr = S_OK;

	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szFileName.c_str(), NULL, NULL, &m_pTexSRV, NULL)))
	{
		return false;
	}

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	m_pd3dDevice->CreateSamplerState(&sd, &m_pSamplerState);
	return true;
}
bool xTexture::Init()
{
	return true;
}
bool xTexture::Frame()
{
	return true;
}
bool xTexture::Render()
{
	return true;
}
bool xTexture::Release()
{
	SAFE_RELEASE(m_pTexSRV);
	SAFE_RELEASE(m_pSamplerState);
	return true;
}

xTexture::xTexture()
{
}


xTexture::~xTexture()
{
}
