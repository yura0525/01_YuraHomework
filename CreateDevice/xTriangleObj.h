#pragma once
#include "xObject.h"

class xTriangleObj : public xObject
{
public:
	HRESULT CreateIndexBuffer(ID3D11Device* pd3dDevice);
public:
	xTriangleObj();
	virtual ~xTriangleObj();
};

