#include "TGameUser.h"
#include "TBasisSys.h"
#include "TObjectMgr.h"
#include "TDebugString_1.h"

bool TGameUser::Init()
{
	m_iModelType = 0;
	m_fPosX = 400;
	m_fPosY = 500;
	I_ObjectMgr.m_ObjectList[m_iModelType].MoveObject(m_fPosX, m_fPosY);
	return true;
}
bool TGameUser::Render()
{
	I_ObjectMgr.m_ObjectList[m_iModelType].RotateBlt();
	return true;
}
bool TGameUser::Frame()
{
	switch (m_Direction)
	{
	case VK_DOWN: MoveDown(0); break;
	case VK_UP: MoveUp(0); break;
	case VK_LEFT: MoveLeft(0); break;
	case VK_RIGHT: MoveRight(0); break;
	}
	I_ObjectMgr.m_ObjectList[m_iModelType].MoveObject(m_fPosX, m_fPosY);
	return true;
}
bool TGameUser::Release()
{
	return true;
}
void TGameUser::MoveLeft(int iUserID)
{
	I_DebugStr.DisplayText(const_cast<char*>("\n####TGameUser::MoveLeft\n"));
	float fPos = m_fPosX;
	fPos -= 100.0f * g_fSecPerFrame;
	if (fPos < 0) fPos = 0;
	m_fPosX = fPos;
}
void TGameUser::MoveRight(int iUserID)
{
	I_DebugStr.DisplayText(const_cast<char*>("\n####TGameUser::MoveRight\n"));
	float fPos = m_fPosX;
	fPos += 100.0f * g_fSecPerFrame;
	if (fPos > 780) fPos = 780;
	m_fPosX = fPos;
}
void TGameUser::MoveUp(int iUserID)
{
	I_DebugStr.DisplayText(const_cast<char*>("\n####TGameUser::MoveUp\n"));
	float fPos = m_fPosY;
	fPos -= 100.0f * g_fSecPerFrame;
	if (fPos < 0) fPos = 0;
	m_fPosY = fPos;
}
void TGameUser::MoveDown(int iUserID)
{
	I_DebugStr.DisplayText(const_cast<char*>("\n####TGameUser::MoveDown\n"));
	float fPos = m_fPosY;
	fPos += 100.0f * g_fSecPerFrame;
	if (fPos > 590) fPos = 590;
	m_fPosY = fPos;
}
TGameUser::TGameUser(void)
{
	m_Direction = 0;
}

TGameUser::~TGameUser(void)
{
}