#pragma once
#include "TStd.h"

class TLogManager : public TSingleton<TLogManager>
{
private:
	friend class TSingleton<TLogManager>;

public:
	void OutputLog(const char* pText);
protected:
	TLogManager();
public:
	virtual ~TLogManager();
};

#define I_LOGMANAGER TLogManager::GetInstance()

