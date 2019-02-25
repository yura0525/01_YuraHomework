#include "TTargetPointMgr.h"

std::map<int, TPACKET_TARGETPOINT_INFO>& TTargetPointMgr::Get()
{
	return m_TargetPointMap;
}

ostream& operator<<(ostream& stream, TTargetPointMgr& tpList)
{
	size_t iNumCount = tpList.m_TargetPointMap.size();
	tBlockWrite<size_t>(stream, iNumCount);
	for (auto& tp : tpList.m_TargetPointMap)
	{
		tBlockWrite<TPACKET_TARGETPOINT_INFO>(stream, tp.second);
	}
	return stream;
}

istream& operator>>(istream& stream, TTargetPointMgr& tpList)
{
	tpList.m_TargetPointMap.clear();
	size_t iNumCount;
	tBlockRead<size_t>(stream, iNumCount);

	TPACKET_TARGETPOINT_INFO tp;
	for (int iIndex = 0; iIndex < iNumCount; iIndex++)
	{
		tBlockRead<TPACKET_TARGETPOINT_INFO>(stream, tp);
		tpList.m_TargetPointMap[iIndex] = tp;
	}
	return stream;
}

TTargetPointMgr::TTargetPointMgr()
{
}


TTargetPointMgr::~TTargetPointMgr()
{
}
