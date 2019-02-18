#include "TServerObj_1.h"

TServerObj_1::TServerObj_1(void)
{
	InitializeCriticalSection(&m_CS);
}

TServerObj_1::~TServerObj_1(void)
{
	DeleteCriticalSection(&m_CS);
}
