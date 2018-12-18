#include "stdafx.h"
#include "TStreamPacket.h"
#include "XUser.h"

//한번에 2048로 받았기 때문에 패킷 단위로 잘라서 저장해야한다.
void TStreamPacket::Put(char* recvBuffer, int iSize, XUser* pUser)
{
	//받는 버퍼의 용량이 부족하면
	//짜투리로 남은 부분을 strTemp에 저장하고
	//모두 지운 다음에 다시 strTemp 남은 부분을 앞부분에 저장한다.
	if (m_dwWritePos + iSize >= MAX_RECV_SIZE)
	{
		char strTemp[MAX_RECV_SIZE] = { 0, };
		memcpy(strTemp, &m_RecvBuffer[m_dwStartPos], m_dwReadPos);
		ZeroMemory(&m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
		memcpy(&m_RecvBuffer, &strTemp, m_dwReadPos);
		m_pPacket = (P_UPACKET)m_RecvBuffer;
		m_dwStartPos = 0;
		m_dwWritePos = m_dwReadPos;
	}

	memcpy(&m_RecvBuffer[m_dwWritePos], recvBuffer, iSize);
	m_dwWritePos += iSize;
	m_dwReadPos += iSize;

	if (m_dwReadPos < PACKET_HEADER_SIZE)
		return;

	//패킷의 시작
	m_pPacket = (P_UPACKET)&m_RecvBuffer[m_dwStartPos];
	//리시브된 패킷이 최소한 4바이트 이상이다. 최소 하나의 패킼ㅅ이 들어왔다.
	if (m_dwReadPos >= m_pPacket->ph.len + PACKET_HEADER_SIZE)
	{
		//패킷을 잘라서 넣는다.
		do
		{
			T_PACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			packet.pUser = pUser;
			memcpy(&packet, &m_RecvBuffer[m_dwStartPos], m_pPacket->ph.len + PACKET_HEADER_SIZE);
			I_User.AddPacket(packet);

			//1개의 패킷을 처리하고 남은 잔여량 크기
			m_dwReadPos -= m_pPacket->ph.len + PACKET_HEADER_SIZE;
			//패킷의 시작 위치
			m_dwStartPos += m_pPacket->ph.len + PACKET_HEADER_SIZE;

			//잔여량이 패킷헤더보다 작을 경우
			if (m_dwReadPos < PACKET_HEADER_SIZE)
				break;

			m_pPacket = (P_UPACKET)&m_RecvBuffer[m_dwStartPos];
		} while (m_dwReadPos >= m_pPacket->ph.len + PACKET_HEADER_SIZE);
	}

}
TStreamPacket::TStreamPacket()
{
	m_dwWritePos = 0;
	m_dwReadPos = 0;
	m_dwStartPos = 0;
}


TStreamPacket::~TStreamPacket()
{
}
