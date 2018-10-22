#pragma once
#include "TStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

using namespace FMOD;

const int g_iMaxSound = 32;

class TSound
{
public:
	static TSound* GetInstance()
	{
		static TSound sound;
		return &sound;
	}

public:
	int					m_iSoundList;

	FMOD::System*		m_pSystem;
	FMOD::Sound*		m_pSound[g_iMaxSound];
	FMOD::Channel*		m_pChannel[g_iMaxSound];
	TCHAR				m_szBuffer[MAX_PATH];

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	int Load(const char* pLoadName, bool bPlay);
	
	void PlayEffect(int iIndex);

	void Play(int iIndex, bool bPlay =  true, bool bLoop = false);
	void Stop(int iIndex);
	void Paused(int iIndex);
	void Volume(int iIndex, float fVolume, bool bUp);
private:
	TSound();
public:
	virtual ~TSound();
};

