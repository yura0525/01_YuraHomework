#pragma once
#include "TServerStd.h"
#include "TProtocol.h"
#include "TServerObj.h"

typedef std::map<tGUID, TCharacter, GUIDComparer>::iterator m_CharMapItor;
typedef std::map<tGUID, TUser*, GUIDComparer>::iterator		m_UserMapItor;
typedef std::map<tGUID, TUser*, GUIDComparer>				m_UserMapList;

class TServer : public TSingleton<TServer>, public TServerObj
{
private:
	friend class TSingleton<TServer>;

public:
	TServer();
	virtual ~TServer();
};

#define I_Server TServer::GetInstance()
