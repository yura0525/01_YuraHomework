#include "stdafx.h"
#include "TStreamPacket.h"
#include "XUser.h"

//�ѹ��� 2048�� �޾ұ� ������ ��Ŷ ������ �߶� �����ؾ��Ѵ�.
void TStreamPacket::Put(char* recvBuffer, int iSize, XUser* pUser)
{
	//�޴� ������ �뷮�� �����ϸ�
	//¥������ ���� �κ��� strTemp�� �����ϰ�
	//��� ���� ������ �ٽ� strTemp ���� �κ��� �պκп� �����Ѵ�.
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

	//��Ŷ�� ����
	m_pPacket = (P_UPACKET)&m_RecvBuffer[m_dwStartPos];
	//1���� ��Ŷ �����ŭ �޾Ҵٸ�
	if (m_dwReadPos >= m_pPacket->ph.len)
	{
		do
		{
			T_PACKET add;
			ZeroMemory(&add, sizeof(add));
			//�����ϴ���.
		}
	}

}
TStreamPacket::TStreamPacket()
{
}


TStreamPacket::~TStreamPacket()
{
}
