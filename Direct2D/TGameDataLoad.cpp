#include "TGameDataLoad.h"

bool TGameDataLoad::Init()
{
	GameDataLoad(L"../data/Resource/GameData.txt");
	return true;
}

bool TGameDataLoad::GameDataLoad(const TCHAR* pszFileName)
{
	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumCount = 0;
	FILE* fp_src;

	_wfopen_s(&fp_src, pszFileName, _T("rt"));

	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumCount);
	
	for (int iCnt = 0; iCnt < iNumCount; iCnt++)
	{
		int iNumValue = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumValue);

		wstring temp(pTemp);
		if (temp == wstring(L"g_iMAX_NPC_COUNT"))
		{
			g_iMAX_NPC_COUNT = iNumValue;
		}
		else if (temp == wstring(L"g_MOVE_NPC_SPEED"))
		{
			g_MOVE_NPC_SPEED = iNumValue;
		}
		else if (temp == wstring(L"g_NPC_WIDTH_GAP"))
		{
			g_NPC_WIDTH_GAP = iNumValue;
		}
		else if (temp == wstring(L"g_INIT_NPC_POSY"))
		{
			g_INIT_NPC_POSY = iNumValue;
		}
		else if (temp == wstring(L"g_INIT_NPC_POSX"))
		{
			g_INIT_NPC_POSX = iNumValue;
		}
		else if (temp == wstring(L"g_HERO_DAMAGE_TIME_GAP"))
		{
			g_HERO_DAMAGE_TIME_GAP = iNumValue;
		}
		else if (temp == wstring(L"g_INIT_HERO_HP_GAP_POSY"))
		{
			g_INIT_HERO_HP_GAP_POSY = iNumValue;
		}
		else if (temp == wstring(L"g_HERO_MAXHP"))
		{
			g_HERO_MAXHP = iNumValue;
		}
		else if (temp == wstring(L"g_INIT_HERO_POSY"))
		{
			g_INIT_HERO_POSY = iNumValue;
		}
		else if (temp == wstring(L"g_HPBAR_WIDTH"))
		{
			g_HPBAR_WIDTH = iNumValue;
		}
		else if (temp == wstring(L"g_HPBAR_HEIGHT"))
		{
			g_HPBAR_HEIGHT = iNumValue;
		}
		else if (temp == wstring(L"g_EFFECT_HERO_SPRITE"))
		{
			g_EFFECT_HERO_SPRITE = iNumValue;
		}
		else if (temp == wstring(L"g_EFFECT_NPC_SPRITE"))
		{
			g_EFFECT_NPC_SPRITE = iNumValue;
		}
		else if (temp == wstring(L"g_EFFECT_NPC_MAX_COUNT"))
		{
			g_EFFECT_NPC_MAX_COUNT = iNumValue;
		}
		else if (temp == wstring(L"g_NPC_REGENTIME"))
		{
			g_NPC_REGENTIME = iNumValue;
		}
		else if (temp == wstring(L"g_EFFECT_NPC_REGENTIME"))
		{
			g_EFFECT_NPC_REGENTIME = iNumValue;
		}
		else if (temp == wstring(L"g_TOTAL_GAMETIME"))
		{
			g_TOTAL_GAMETIME = iNumValue;
		}
		else if (temp == wstring(L"g_STAGE_MAX_LEVEL"))
		{
			g_STAGE_MAX_LEVEL = iNumValue;
		}
		else if (temp == wstring(L"g_ITEM_DROP_SPEED"))
		{
			g_ITEM_DROP_SPEED = iNumValue;
		}
		else if (temp == wstring(L"g_ITEM_DROP_PERCENT"))
		{
			g_ITEM_DROP_PERCENT = iNumValue;
		}
		else if (temp == wstring(L"g_NPC_INITHP"))
		{
			g_NPC_INITHP = iNumValue;
		}
	}
	fclose(fp_src);

	return true;
}

TGameDataLoad::TGameDataLoad()
{
}


TGameDataLoad::~TGameDataLoad()
{
}
