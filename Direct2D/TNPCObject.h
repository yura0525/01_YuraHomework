#pragma once
#include "xStd.h"
#include "xObject.h"

class TNPCObject : public xObject
{
public:
	bool Init();
	bool Frame();
public:
	TNPCObject();
	virtual ~TNPCObject();
};

