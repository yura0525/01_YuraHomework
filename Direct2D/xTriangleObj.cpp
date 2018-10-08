#include "xTriangleObj.h"

HRESULT xTriangleObj::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	m_indexList.push_back(0);
	m_indexList.push_back(1);
	m_indexList.push_back(2);

	return xObject::CreateIndexBuffer(pd3dDevice);
}

xTriangleObj::xTriangleObj()
{
}


xTriangleObj::~xTriangleObj()
{
}
