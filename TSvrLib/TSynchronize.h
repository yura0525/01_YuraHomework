#pragma once
#include "TSvrObject.h"

class TSynchronize
{
public:
	TSvrObject* m_pSerObj;
public:
	TSynchronize(TSvrObject* pObj);
	virtual ~TSynchronize();
};

