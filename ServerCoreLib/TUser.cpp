#include "TUser.h"
#include "TDebugString.h"

ostream& operator<<(ostream& stream, TActiveCharacter& info)
{
	{
		TSynchronize sync(&info);
		stream << info.m_List.size();
		for (auto& data : info.m_List)
		{
			stream << data.first;
			stream << data.second;
		}
	}
	return stream;
}
istream& operator>>(istream& stream, TActiveCharacter& info)
{
	{
		TSynchronize sync(&info);
		info.m_List.clear();

		size_t nPlayer = 0;
		tGUID		tGuid;
		TCharacter	tChar;
		stream >> nPlayer;
		for (int i = 0; i < nPlayer; i++)
		{
			stream >> tGuid;
			stream >> tChar;
			info.m_List.insert(make_pair(tGuid, tChar));
		}
	}
	return stream;
}

TCharacterList& TActiveCharacter::Get()
{
	return m_List;
}


ostream& operator<<(ostream& stream, TUser& info)
{
	stream << info.m_Guid;
	stream << info.m_GuidActiveCharacter;
	stream << info.m_Name << endl;
	stream << info.m_CharacterList.size();

	for (auto& tChar : info.m_CharacterList)
	{
		stream << tChar.first;
		stream << tChar.second;
	}
	return stream;
}
istream& operator>>(istream& stream, TUser& info)
{
	int iNumCharacter = 0;
	tGUID tGuid;
	TCharacter tChar;
	info.m_CharacterList.clear();

	stream >> info.m_Guid;
	stream >> info.m_GuidActiveCharacter;
	stream >> info.m_Name;
	stream >> iNumCharacter;

	for (int i = 0; i < iNumCharacter; i++)
	{
		stream >> tGuid;
		stream >> tChar;
		info.m_CharacterList[tGuid] = tChar;
	}
	return stream;
}

TCharacterList&	TUser::GetCharacterList()
{
	return m_CharacterList;
}
void TUser::SetActiveCharacter(TCharacter* pChar, bool bAlive)
{
	m_pCharacter = pChar;
	if (m_pCharacter != NULL)
	{
		m_pCharacter->m_Info.bAlive = bAlive;
	}
}
void TUser::Dispatch(DWORD dwByteSize, LPOVERLAPPED ov)
{
	OVERLAPPED2* pOV = (OVERLAPPED2*)ov;
	if (dwByteSize == 0)
		return;
	if (pOV->m_iFlags == OVERLAPPED2::MODE_RECV)
	{
		m_StreamPacket.Put(m_wsaRecvBuffer.buf, dwByteSize, this);
	}
	WaitForPacketReceive();
}
void TUser::WaitForPacketReceive()
{
	int iRet = 0;
	DWORD cbTransferred, flags = 0;
	m_OV.m_iFlags = OVERLAPPED2::MODE_RECV;
	ZeroMemory(m_recvBuffer, sizeof(char) * MAX_BUFFER);

	m_wsaRecvBuffer.buf = m_recvBuffer;
	m_wsaRecvBuffer.len = MAX_BUFFER - 1;
	iRet = WSARecv(m_Socket, &m_wsaRecvBuffer, 1, &cbTransferred, &flags, (LPOVERLAPPED)&m_OV, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			I_DebugStr.T_ERROR();
		}
	}
}
void TUser::Release()
{

}


TUser::TUser()
{
	m_ConnectType = CONNECT_TYPE::eNOCONNECT;
	m_iType = 0;
}


TUser::~TUser()
{
}