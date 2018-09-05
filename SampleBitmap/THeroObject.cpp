#include "THeroObject.h"
#include "TInput.h"
#include "TCollision.h"

POINT	g_pHeroPos;
bool THeroObject::Frame()
{
	//Hero의 움직임 처리
	if (I_Input.Key('W'))
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('S'))
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('A'))
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('D'))
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
	}

	//위치값이나 충돌박스 수정.
	TObject::Frame();

	return true;
}

bool THeroObject::Render()
{
	TObject::Render();

	// fade out<--> fadeIn
	/*if (bFadeStart && FadeOut())
	{
		bFadeStart = false;
	}

	if (!bFadeStart && FadeIn())
	{
		bFadeStart = true;
	}*/

	g_pHeroPos.x = m_pos.x;
	g_pHeroPos.y = m_pos.y;
	return true;
}
bool THeroObject::FadeOut()
{
	m_fAlpha -= g_fSecPerFrame * 100;
	if (m_fAlpha < 0.0f)
	{
		m_fAlpha = 0.0f;
		return true;
	}

	return false;
}
bool THeroObject::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 100;
	if (m_fAlpha > 255.0f)
	{
		m_fAlpha = 255.0f;
		return true;
	}

	return false;
}
void THeroObject::ProcessDamage(int damage)
{
	TObject::ProcessDamage(damage);
}

THeroObject::THeroObject()
{
	bFadeStart = false;
}

THeroObject::~THeroObject()
{
}
