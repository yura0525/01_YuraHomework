#pragma once
#include "xStd.h"
class TGameDataLoad : public TSingleton<TGameDataLoad>
{
public:
	int g_iMAX_NPC_COUNT;
	float g_MOVE_NPC_SPEED;
	int g_NPC_WIDTH_GAP;

	float g_INIT_NPC_POSY;
	float g_INIT_NPC_POSX;

	float g_HERO_DAMAGE_TIME_GAP;
	int g_INIT_HERO_HP_GAP_POSY;
	int g_HERO_MAXHP;
	int g_INIT_HERO_POSY;

	int g_HPBAR_WIDTH;
	int g_HPBAR_HEIGHT;

	int g_EFFECT_HERO_SPRITE;
	int g_EFFECT_NPC_SPRITE;
	int g_EFFECT_NPC_MAX_COUNT;

	float g_NPC_REGENTIME;
	float g_EFFECT_NPC_REGENTIME;
	float g_TOTAL_GAMETIME;
	int g_STAGE_MAX_LEVEL;
	float g_ITEM_DROP_SPEED;
	int g_ITEM_DROP_PERCENT;
	int g_NPC_INITHP;
	friend class TSingleton<TGameDataLoad>;

public:
	bool Init();
	bool GameDataLoad(const TCHAR* pszFileName);

public:
	TGameDataLoad();
	virtual ~TGameDataLoad();
};

#define I_GameDataLoad TGameDataLoad::GetInstance()