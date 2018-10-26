#pragma once
#include "xStd.h"
#include "xObject.h"
typedef std::vector<RECT> RECT_ARRAY;

class TNPCObject : public xObject
{
public:
	bool Init();
	bool Frame();
public:
	TNPCObject();
	virtual ~TNPCObject();
};

class TNPCMgr : public TSingleton<TNPCMgr>
{
public:
	friend class TSingleton<TNPCMgr>;
	std::vector<RECT_ARRAY>			m_rtSpriteList;

public:
	bool SpriteDataLoad(const TCHAR* pszFileName);

protected:
	TNPCMgr();
public:
	~TNPCMgr();
};

#define I_NPCMgr TNPCMgr::GetInstance()

