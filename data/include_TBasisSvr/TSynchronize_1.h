#pragma once
#include "TServerObj_1.h"

class TSynchronize_1
{
private:
	TServerObj_1 * m_pServerObj;
public:
	TSynchronize_1(TServerObj_1* pSvrObj);
	virtual ~TSynchronize_1(void);
};

