#pragma once
#include "TCharacter.h"
#include "TThread.h"

class TNPC : public TCharacter
{
private:
	float		m_fTraceRange;			// 추격 범위
	float		m_fHitRange;			// 타격 범위
	float		m_fHitValue;			// 타격 포인트
public:
	void SendMove(TCharacter& pDest);
	void SendAttack(TCharacter& pDest);
	void MoveTo(TCharacter& target);
	void HitPlayer(TCharacter& target);
	void Damaged(float damage);
	bool IsAlive();
	bool IsAttack();
	bool MoveChecker(const TCharacter& target);
	bool AttackChecker(const TCharacter& target);
	void SetLocation(float x, float y, float z);
public:
	TNPC();
	virtual ~TNPC();
};

class TNPCManager : public TThread
{
private:
	map<tGUID, TNPC, GUIDComparer>	m_NPCList;
	TTargetPointMgr					m_targetPointMgr;
	TCharacter*						m_pTargetChar;
public:
	void Clear();
public:
	bool Init();
	bool Run();
	bool Set();
	bool Release();
public:
	void SyncNPC();
	void ResetTargetPoint();
	void SendAttack(TNPC* pNPC, TCharacter* pDest);
	void SendMove(TNPC* pNPC, TCharacter* pDest);
	void Add(FVector vPosition, FRotator vRotation = FRotator::ZeroRotator, FVector vVelocity = FVector::ZeroVector);
public:
	friend ostream& operator<<(ostream& stream, TNPCManager& npcList);
	friend istream& operator>>(istream& stream, TNPCManager& npcList);

public:
	TNPCManager();
	~TNPCManager();
};