#pragma once
#include "THPBarObject.h"

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

class THeroMgr : public TSingleton<THeroMgr>
{
public:
	friend class TSingleton<THeroMgr>;

	THeroObject		m_Hero;
	THPBarObject	m_HPBar;
	xObject*		m_pLifeIcon;
	int				m_iCurrentLevel;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	void Reset();

protected:
	THeroMgr();
public:
	~THeroMgr();
};

#define I_HeroMgr THeroMgr::GetInstance()