#include "stdafx.h"
#include "TSvrObject.h"


TSvrObject::TSvrObject()
{
	InitializeCriticalSection(&m_cs);
}


TSvrObject::~TSvrObject()
{
	DeleteCriticalSection(&m_cs);
}
