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
	//���ú�� ��Ŷ�� �ּ��� 4����Ʈ �̻��̴�. �ּ� �ϳ��� �еf���� ���Դ�.
	if (m_dwReadPos >= m_pPacket->ph.len + PACKET_HEADER_SIZE)
	{
		//��Ŷ�� �߶� �ִ´�.
		do
		{
			T_PACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			packet.pUser = pUser;
			memcpy(&packet, &m_RecvBuffer[m_dwStartPos], m_pPacket->ph.len + PACKET_HEADER_SIZE);
			I_User.AddPacket(packet);

			//1���� ��Ŷ�� ó���ϰ� ���� �ܿ��� ũ��
			m_dwReadPos -= m_pPacket->ph.len + PACKET_HEADER_SIZE;
			//��Ŷ�� ���� ��ġ
			m_dwStartPos += m_pPacket->ph.len + PACKET_HEADER_SIZE;

			//�ܿ����� ��Ŷ������� ���� ���
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
