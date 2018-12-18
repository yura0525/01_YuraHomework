#pragma once
class TSvrObject
{
public:
	CRITICAL_SECTION m_cs;

public:
	TSvrObject();
	virtual ~TSvrObject();
};

