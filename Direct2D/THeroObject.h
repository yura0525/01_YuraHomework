#pragma once
#include "xObject.h"

class THeroObject : public xObject
{
public:
	bool	bFadeStart;
public:
	bool Frame();
	virtual bool Render() override;
	virtual bool FadeOut() override;
	virtual bool FadeIn() override;

public:
	THeroObject();
	virtual ~THeroObject();
};

