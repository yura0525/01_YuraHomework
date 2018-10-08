#include "TLogManager.h"
#include <fstream>

void TLogManager::OutputLog(const char* pText)
{
	std::fstream fp("log.txt", std::ios::out);
	fp << pText;
}

TLogManager::TLogManager()
{
}


TLogManager::~TLogManager()
{
}
