#pragma once

#include "TStd.h"
#include "TFlightObject.h"

class TNPCObject : public TFlightObject
{

public:
	bool Frame();

public:
	TNPCObject();
	virtual ~TNPCObject();
};

