#pragma once
#include "TServerStd.h"

class TServerObj_1
{
public:
	CRITICAL_SECTION   m_CS;
public:
	TServerObj_1(void);
	virtual ~TServerObj_1(void);
};
