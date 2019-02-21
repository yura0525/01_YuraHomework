#include "TServerObj.h"



TServerObj::TServerObj()
{
	InitializeCriticalSection(&m_CS);
}


TServerObj::~TServerObj()
{
	DeleteCriticalSection(&m_CS);
}
