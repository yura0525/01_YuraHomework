#pragma once
#include "TServerStd.h"
#include "TSvrObject.h"

class TLogError : public TSingleton<TLogError>, public TSvrObject
{
	friend class TSingleton<TLogError>;
public:
	void T_ERROR();
public:
	TLogError();
	virtual ~TLogError();
};
#define I_LOG TLogError::GetInstance()

