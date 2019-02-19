#include "TNpc.h"
#include <thread>
#include "TServer_1.h"

void TNpc::MoveTo(const TCharacter& target)
{
	if (target.m_fPos[0] > m_fPos[0]) 	m_fPos[0] += m_fMovePoint;
	if (target.m_fPos[0] < m_fPos[0])	m_fPos[0] -= m_fMovePoint;
	if (target.m_fPos[1] > m_fPos[1])	m_fPos[1] += m_fMovePoint;
	if (target.m_fPos[1] < m_fPos[1])	m_fPos[1] -= m_fMovePoint;
	if (target.m_fPos[2] > m_fPos[2])	m_fPos[2] += m_fMovePoint;
	if (target.m_fPos[2] < m_fPos[2])	m_fPos[2] -= m_fMovePoint;
}
void TNpc::HitPlayer(TCharacter& target)
{
	std::thread t([&]()
	{
		// 1초에 한번씩 때리도록
		m_bAttacking = true;
		target.m_fHealth -= m_fHitPoint;
		std::this_thread::sleep_for(1s);
		m_bAttacking = false;
	});
	t.detach();
}
void TNpc::Damaged(float damage)
{
	m_fHealth -= damage;
}
bool TNpc::IsAlive()
{
	if (m_fHealth <= 0)
		return false;

	return true;
}
bool TNpc::IsAttacking()
{
	return m_bAttacking;
}
bool TNpc::IsPlayerInTraceRange(const TCharacter& target)
{
	if (abs(target.m_fPos[0] - m_fPos[0]) < m_fTraceRange && abs(target.m_fPos[1] - m_fPos[1]) < m_fTraceRange)
		return true;

	return false;
}
bool TNpc::IsPlayerInHitRange(const TCharacter& target)
{
	if (abs(target.m_fPos[0] - m_fPos[0]) < m_fHitRange && abs(target.m_fPos[1] - m_fPos[1]) < m_fHitRange)
		return true;

	return false;
}
void TNpc::SetLocation(float x, float y, float z)
{
	m_fPos[0] = x;
	m_fPos[1] = y;
	m_fPos[2] = z;
}

TNpc::TNpc()
{
	m_fTraceRange = 700.0f;
	m_fHitRange = 200.0f;
	m_fMovePoint = 5.0f;
	m_fHitPoint = 0.1f;
}


TNpc::~TNpc()
{
}


bool TNpcManager::Init()
{
	// 몬스터 초기화	
	TNpc npc0;
	npc0.m_fPos[0] = 1;
	npc0.m_fPos[1] = 2;
	npc0.m_fPos[2] = 3;
	npc0.m_fAngle[0] = 0.0f;
	npc0.m_fAngle[1] = 0.0f;
	npc0.m_fAngle[2] = 0.0f;
	npc0.m_fVelocity[0] = 0.0f;
	npc0.m_fVelocity[1] = 0.0f;
	npc0.m_fVelocity[2] = 0.0f;
	npc0.m_fHealth = 100.0f;
	npc0.m_bAlive = true;
	npc0.m_bAttacking = false;
	npc0.m_iID = 0;
	m_NpcList[npc0.m_iID] = npc0;
	return true;
}
bool TNpcManager::Run()
{
	//몬스터 초기화
	Init();
	int count = 0;
	//로직 시작
	while (true)
	{
		for (auto& npc : m_NpcList)
		{
			auto& monster = npc.second;
			for (auto& player : I_Server.m_UserList)
			{
				TCharacter* playCharacter = player.second->m_pCharacter;
				if (player.second->m_pCharacter != NULL)
				{
					if (!player.second->m_pCharacter->m_bAlive || !monster.m_bAlive)
						continue;
				}
			}
		}
	}
}
bool TNpcManager::Set()
{
	CreateThread();
	return true;
}