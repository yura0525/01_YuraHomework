#pragma once
#include "TServerStd.h"

class TServerObj
{
public:
	CRITICAL_SECTION m_CS;
public:
	TServerObj();
	virtual ~TServerObj();
};

