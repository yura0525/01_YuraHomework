#pragma once
#include "TServerObj_2.h"

class TSynchronize_2
{
private:
	TServerObj_2 * m_pServerObj;

public:
	TSynchronize_2(TServerObj_2 * pSvrObj);
	virtual ~TSynchronize_2();
};

