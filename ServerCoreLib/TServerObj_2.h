#pragma once
#include <Windows.h>

class TServerObj_2
{
public:
	CRITICAL_SECTION m_CS;
public:
	TServerObj_2();
	virtual ~TServerObj_2();
};

