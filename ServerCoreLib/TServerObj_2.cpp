#include "TServerObj_2.h"



TServerObj_2::TServerObj_2()
{
	InitializeCriticalSection(&m_CS);
}


TServerObj_2::~TServerObj_2()
{
	DeleteCriticalSection(&m_CS);
}
