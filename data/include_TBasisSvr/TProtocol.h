#pragma once
#include <windows.h>
#pragma  pack(push, 1 )

typedef struct {
	WORD	len;	// msg ����Ʈ ũ��
	WORD	type;	// � ��Ŷ�̴�.
}PACKET_HEADER;

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

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000

#pragma pack(pop)