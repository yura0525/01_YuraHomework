#pragma once
#include "TServerObj.h"

class TSynchronize
{
private:
	TServerObj * m_pServerObj;

public:
	TSynchronize(TServerObj * pSvrObj);
	virtual ~TSynchronize();
};

