#pragma once
#include <windows.h>

#define PACKET_HEADER_SIZE 4

#pragma pack( push, 1)
typedef struct {
	WORD  len;   // ��Ŷ�� ��ü ����
	WORD  type;  // ��Ŷ�� Ÿ��(����)
				 //WORD  Index; // ��Ŷ�� ������ �ε���
				 //WORD  time;  // ��Ŷ�� ���� �ð�
}PACKET_HEADER;
typedef struct {
	PACKET_HEADER ph;
	char		  msg[2048];
}UPACKET, *P_UPACKET;
///   �ɸ��� ���� ��Ŷ
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[50];
}CHARACTER_INFO;
///   ������ ���� ��Ŷ
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[10];
}ITEM_INFO;
#pragma pack(pop) // ����

// [0] ~ [12][13][14] [14][15][16]
//////////////  ��Ŷ�� Ÿ�Ե� /////////
#define PACKET_CHAT_MSG    1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000
/*
MSG = "�ȳ��ϼ���!"

UPACKET  sendPacket;
sendPacket.ph.len = strlen(MSG) + PACKET_HEADER_SIZE;
sendPacket.ph.type = PACKET_CHAT_MSG;
sendPacket.msg = MSG;

UPACKET  sendPacket;
sendPacket.ph.len = strlen(CHARACTER_INFO) + PACKET_HEADER_SIZE;
sendPacket.ph.type = PACKET_CREATE_CHARACTER;
sendPacket.msg = CHARACTER_INFO;
// 4 ����Ʈ ��ŷ
struct TData
{
int   iA; // 4
int   iD; // 4
short sB; // 2 �� 2byte
short sC; // 2 �� 2byte
short sC; // 2 �� 2byte
byte  bC; // 3 byte
byte  bC; // 3 byte
};
// 200
UPACKET a[10]; for( send(a[i] )
// a[0]a[1]a[2]a[3] == > ��Ʈ��(�������� ���� �帧 )
char buffer[256];
UPACKET a[10];
while(1)
{
iRecvByte=recv(buffer, 256)
a[0]����, a[1]= 56 + 144byte;�Ϻκи� ����
200-56 = 144 ~ 256  256-144= 12
a[2] = 144~ 244;
a[3] = 244~256=12
}
recvbuffer = recv();
if( strlen(recvbuffer) > PACKET_HEADER_SIZE)
{
// if( strlen(recvbuffer) > packet.len )
{

}
}
*/