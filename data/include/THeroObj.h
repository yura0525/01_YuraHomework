#pragma once
#include "TObject.h"
class THeroObj : public TObject
{
public:
	bool bFadeStart;
public:
	virtual bool Render() override;

public:
	virtual bool FadeOut() override;
	virtual bool FadeIn() override;
public:
	THeroObj();
	virtual ~THeroObj();
};