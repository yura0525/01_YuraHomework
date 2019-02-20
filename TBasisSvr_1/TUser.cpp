#include "TUser.h"
#include "TCharacter.h"


TUser::TUser()
{
	TCharacter defaultChar;
	tGUID		uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	m_CharacterList.insert(make_pair(uuid, defaultChar));
	m_pCharacter = &m_CharacterList[uuid];
	m_pCharacter->m_bAlive = true;
}


TUser::~TUser()
{
}
