#pragma once
#include "xObject.h"

class xPlaneObj : public xObject
{
public:
	HRESULT CreateIndexBuffer(ID3D11Device* pd3dDevice);
public:
	xPlaneObj();
	virtual ~xPlaneObj();
};

