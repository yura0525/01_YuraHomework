#pragma once
#include "TFlightObject.h"

class THeroObject : public TFlightObject
{
public:
	bool Frame();
public:
	void ProcessDamage(int damage);
	void SetMAXHP(int hp);
public:
	THeroObject();
	virtual ~THeroObject();
};

