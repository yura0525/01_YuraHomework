#pragma once
#include "TThread_3.h"
class TWorkThread_1 : public TThread_3
{
public:
	bool		m_bLoop;
public:
	bool		Run();

public:
	TWorkThread_1();
	virtual ~TWorkThread_1();
};

