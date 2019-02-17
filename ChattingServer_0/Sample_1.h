#pragma once
#include "TAcceptor_1.h"
#include "xCore.h"

class Sample_1 : public xCore
{
public:
	TAcceptor_1		m_Acceptor;
public:
	bool			Init();
	bool			PreRender();
	bool			Render();
	bool			PostRender();
	bool			Release();
public:
	Sample_1();
	~Sample_1();
};

