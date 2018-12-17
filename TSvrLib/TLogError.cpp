#include "stdafx.h"
#include "TLogError.h"

void TLogError::T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}
TLogError::TLogError()
{
}


TLogError::~TLogError()
{
}
