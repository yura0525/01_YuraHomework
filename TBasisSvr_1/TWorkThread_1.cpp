#include "TWorkThread_1.h"

bool TWorkThread_1::Init()
{
	//TODO:해야함.
	return true;
}
bool TWorkThread_1::Run()
{
	//TODO:해야함.
	return true;
}
bool TWorkThread_1::Release()
{
	//TODO:해야함.
	return true;
}

TWorkThread_1::TWorkThread_1()
{
	m_bLoop = true;
}


TWorkThread_1::~TWorkThread_1()
{
	m_bLoop = false;
}
