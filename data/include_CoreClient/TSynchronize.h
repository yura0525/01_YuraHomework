#pragma once
#include "TServerStd.h"
class TServerObj
{
public:
	CRITICAL_SECTION m_cs;
public:
	TServerObj();
	virtual ~TServerObj();
};

class TSynchronize : public TServerObj
{
public:
	TServerObj * m_pSerObj;
public:
	TSynchronize(TServerObj * pObj);
	virtual ~TSynchronize();
};

