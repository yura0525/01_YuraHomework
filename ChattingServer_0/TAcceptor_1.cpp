#include "TAcceptor_1.h"

HANDLE m_Mutex;
int clientNumber = 0;
int	allClientSocket[100];


bool TAcceptor_1::Init()
{
	return true;
}
bool TAcceptor_1::Run()
{
	return true;
}
bool TAcceptor_1::Set(int iPort, const char* strAddress)
{
	return true;
}
int	 TAcceptor_1::GetIPList()
{
	return 0;
}
bool TAcceptor_1::Release()
{
	CloseHandle((HANDLE)m_hThread);
	m_bStarted = false;
	return true;
}
TAcceptor_1::TAcceptor_1()
{
	clientNumber = 0;
}


TAcceptor_1::~TAcceptor_1()
{
}
