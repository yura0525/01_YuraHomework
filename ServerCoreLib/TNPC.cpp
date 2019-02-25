#include "TNpc.h"
#include <thread>
#include "TServer.h"

void TNPC::SendMove(TCharacter& pDest)
{
	stringstream data;
	data << *this;
	data << pDest;
	std::stringstream stream = MakePacket(PACKET_NPC_TARGETMOVE, data);
	I_Server.Broadcast(stream);
}
void TNPC::SendAttack(TCharacter& pDest)
{
	stringstream data;
	data << *this;
	data << pDest;
	std::stringstream stream = MakePacket(PACKET_ATTACK_MONSTER, data);
	I_Server.Broadcast(stream);
	I_DebugStr.DisplayText("\n[%s]\n", "PACKET_ATTACK_MONSTER");
}
void TNPC::MoveTo(TCharacter& target)
{
	std::thread t([&]()
	{
		SendMove(target);
		std::this_thread::sleep_for(1s);
	});
	t.detach();
}
void TNPC::HitPlayer(TCharacter& target)
{
	std::thread t([&]()
	{
		SendAttack(target);
		m_Info.bAttack = true;
		target.m_Info.fHealth -= m_fHitValue;
		std::this_thread::sleep_for(1s);
		m_Info.bAttack = false;
	});
	t.detach();
}
void TNPC::Damaged(float damage)
{
	m_Info.fHealth -= damage;
}
bool TNPC::IsAlive()
{
	if (m_Info.fHealth <= 0)
		return false;

	return true;
}
bool TNPC::IsAttack()
{
	return m_Info.bAttack;
}
bool TNPC::MoveChecker(const TCharacter& target)
{
	if (m_Info.vPosition.DistanceTo(target.m_Info.vPosition) <= m_fTraceRange)
	{
		return true;
	}
	return false;
}
bool TNPC::AttackChecker(const TCharacter& target)
{
	if (m_Info.vPosition.DistanceTo(target.m_Info.vPosition) <= m_fHitRange)
	{
		return true;
	}
	return false;
}
void TNPC::SetLocation(float x, float y, float z)
{
	m_Info.vPosition.m_fX = x;
	m_Info.vPosition.m_fY = y;
	m_Info.vPosition.m_fZ = z;
}

TNPC::TNPC()
{
	m_fTraceRange = 700.0f;
	m_fHitRange = 200.0f;
	m_fHitValue = 1.0f;
}


TNPC::~TNPC()
{
}

ostream& operator<<(ostream& stream, TNPCManager& npcList)
{
	{
		TSynchronize sync(&npcList);
		stream << npcList.m_NPCList.size();
		for (auto& npc : npcList.m_NPCList)
		{
			stream << npc.first;
			stream << npc.second;
		}
	}
	return stream;
}
istream& operator>>(istream& stream, TNPCManager& npcList)
{
	{
		TSynchronize sync(&npcList);
		tGUID tGuid;
		TNPC tNPC;
		size_t iNumNPC = 0;
		stream >> iNumNPC;
		for (int i = 0; i < iNumNPC; i++)
		{
			stream >> tGuid;
			stream >> tNPC;
			npcList.m_NPCList[tGuid].m_Info.vPosition = tNPC.m_Info.vPosition;
			npcList.m_NPCList[tGuid].m_Info.vRotation = tNPC.m_Info.vRotation;
			npcList.m_NPCList[tGuid].m_Info.vVelocity = tNPC.m_Info.vVelocity;
		}
	}
	return stream;
}

void TNPCManager::Clear()
{
	m_NPCList.clear();
}
bool TNPCManager::Init()
{
	return true;
}
bool TNPCManager::Run()
{
	Init();
	while (true)
	{
		{
			TSynchronize sync(this);
			for (auto& npc : m_NPCList)
			{
				auto& tNPC = npc.second;
				{
					TSynchronize sync(I_Server.GetPtr());
					for(auto& player : I_Server.GetUserCharacterList().Get())
					{
						TCharacter* playCharacter = &player.second;
						if (!playCharacter->m_Info.bAlive || !tNPC.m_Info.bAlive)
							continue;

						if (!tNPC.m_Info.bAttack)
						{
							if (tNPC.AttackChecker(*playCharacter))
							{
								tNPC.HitPlayer(*playCharacter);
							}
							else if (tNPC.MoveChecker(*playCharacter))
							{
								tNPC.MoveTo(*playCharacter);
							}
						}
					}
				}
			}
			if (m_NPCList.size() > 0)
			{
				SyncNPC();
			}
		}
		Sleep(1);
	}
	Release();
	return true;
}
bool TNPCManager::Set()
{
	CreateThread();
	return true;
}
bool TNPCManager::Release()
{
	{
		TSynchronize sync(this);
		m_NPCList.clear();
	}
	return true;
}
void TNPCManager::SyncNPC()
{
	std::thread t([&]()
	{
		stringstream data;
		data << *this;
		std::stringstream stream = MakePacket(PACKET_SYNC_MONSTER, data);
		I_Server.Broadcast(stream);
		std::this_thread::sleep_for(1s);
	});
	t.detach();
	I_DebugStr.DisplayText("[%s]\n", "PACKET_SYNC_MONSTER");
}
void TNPCManager::ResetTargetPoint()
{
	{
		TSynchronize sync(this);
		m_NPCList.clear();
		for (int iNum = 0; iNum < I_Server.GetTargetPointMgr().Get().size(); iNum++)
		{
			Add(I_Server.GetTargetPointMgr().Get()[iNum].vPosition);
		}
	}
}
void TNPCManager::SendAttack(TNPC* pNPC, TCharacter* pDest)
{

}
void TNPCManager::SendMove(TNPC* pNPC, TCharacter* pDest)
{
	if (m_pTargetChar != NULL)
		return;

	m_pTargetChar = pDest;
	stringstream data;
	data << *pNPC;
	data << *pDest;
	std::stringstream stream = MakePacket(PACKET_NPC_TARGETMOVE, data);
	I_Server.Broadcast(stream);
}
void TNPCManager::Add(FVector vPosition, FRotator vRotation, FVector vVelocity)
{
	{
		TSynchronize sync(this);
		TNPC npc;
		npc.m_Info.vPosition = vPosition;
		npc.m_Info.vRotation = vRotation;
		npc.m_Info.vVelocity = vVelocity;
		npc.m_Info.fHealth = 1.0f;
		npc.m_Info.bAlive = true;
		npc.m_Info.bAttack = false;
		RPC_STATUS ret_val = ::UuidCreate(&npc.m_Info.tGuid);
		m_NPCList[npc.m_Info.tGuid] = npc;
	}
}

TNPCManager::TNPCManager()
{
	m_pTargetChar = NULL;
}
TNPCManager::~TNPCManager()
{

}