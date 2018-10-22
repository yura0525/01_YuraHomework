#pragma once
#include "xObject.h"
class TBKObject : public xObject
{
public:

	virtual bool FadeOut() override;
	virtual bool FadeIn() override;

	TBKObject();
	virtual ~TBKObject();
};

