#pragma once
#include "xStd.h"
#include "THPBarObject.h"

typedef std::vector<RECT> RECT_ARRAY;
enum eNPCTYPE
{
	eGoldDragon,
	ePinkDragon,
	eGreenDragon,
	eBlueDragon,
	eRedDragon,
	ePurpleDragon,
	eBlackDragon,
	eMaxDragon,
};

class TNPCObject : public xObject
{
public:
	eNPCTYPE		m_eNPCType;
	THPBarObject	m_HPBar;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	TNPCObject(eNPCTYPE eType);
	virtual ~TNPCObject();
};

class TNPCMgr : public TSingleton<TNPCMgr>
{
public:
	friend class TSingleton<TNPCMgr>;

	list<TNPCObject*>				m_NPCList;
	std::vector<RECT_ARRAY>			m_rtSpriteList;
public:
	int								m_iMaxNPCCount;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	void Reset();
	void NPCRegenAlarm();
	void DeleteNPCList();
public:
	bool SpriteDataLoad(const TCHAR* pszFileName);

protected:
	TNPCMgr();
public:
	~TNPCMgr();
};

#define I_NPCMgr TNPCMgr::GetInstance()

