#pragma once
#include <windows.h>
#pragma  pack(push, 1 )

typedef struct {
	WORD	len;	// msg ����Ʈ ũ��
	WORD	type;	// � ��Ŷ�̴�.
	WORD	iotype;	// ��Ŷ ����� ����
	DWORD	time;	// ��Ŷ ���� �ð�
}PACKET_HEADER;

#define PACKET_HEADER_SIZE 10

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;

// ����
#define PACKET_CHAT_NAME_REQ 1000 // ��ȭ�� �Է��ض�.
// Ŭ���̾�Ʈ
#define PACKET_CHAT_NAME_ACK 1001 // ��ȭ�� �Է� ����.
// ���� ä�� �޼���
#define PACKET_CHAT_MSG 1002 // ��ȭ�� �Է� ����.
#define PACKET_LOGIN			1003 // �α���
#define PACKET_LOGOUT			1004 // �α׾ƿ�

#define PACKET_CREATE_ACCOUNT	2000 // ��������
#define PACKET_DELETE_ACCOUNT	2001 // ��������

typedef struct
{
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000 // �ɸ��� ��ġ �̵� ����

typedef struct
{
	int m_iID;
	float m_fPos[3];
	float m_fAngle[3];
	float m_fVelocity[3];
	float m_fHealth;
	bool m_bAttacking;
	bool m_bAlive;
}TPACKET_CHARACTER_INFO;

#define PACKET_CREATE_CHARACTER 4000 // �ɸ��� ����
#define PACKET_DELETE_CHARACTER 4001 // �ɸ��� ����
#define PACKET_HIT_CHARACTER	4002 // �ɸ��� �ǰ� ����
#define PACKET_HIT_MONSTER		4003 // ���� �ǰ� ����
#define PACKET_ATTACK_CHARACTER	4004 // �ɸ��� ���� ����
#define PACKET_ATTACK_MONSTER	4005 // ���� ���� ����
#define PACKET_DEAD_CHARACTER	4006 // �ɸ��� ����
#define PACKET_DEAD_MONSTER		4007 // ���� ����
#define PACKET_DAMAGE_CHARACTER	4008 // �ɸ��� ������ ����
#define PACKET_DAMAGE_MONSTER	4009 // ���� ������ ����
#define PACKET_SPAWN_CHARACTER	4010 // �ɸ��� ����
#define PACKET_SPAWN_MONSTER	4011 // ���� ����
#define PACKET_SYNC_CHARACTER	4012 // �ɸ��� ����ȭ
#define PACKET_SYNC_MONSTER		4013 // ���� ����ȭ

#pragma pack(pop)