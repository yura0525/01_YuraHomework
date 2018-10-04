#pragma once
#include "xStd.h"

class xTexture
{
public:
	T_STR m_szName;

public:
	ID3D11Device * m_pd3dDevice;		//����̽� ��ü

	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11SamplerState*			m_pSamplerState;

public:
	bool Load(ID3D11Device* pd3dDevice, T_STR szFileName);

public:
	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����

public:
	xTexture();
	virtual ~xTexture();
};

