#pragma once
#include "FVector.h"
using namespace std;

enum class CONNECT_TYPE
{
	eNOCONNECT,		//UMETA(DisplayName = "NoConnect"),
	eCONNECT,		//UMETA(DisplayName = "Connect"),
	eLOGIN,			//UMETA(DisplayName = "Login"),
	eLOGOUT,		//UMETA(DisplayName = "Logout"),
	eLOBBYLOGIN,	//UMETA(DisplayName = "Lobby-Login"),
	eLOBBYLOGOUT,	//UMETA(DisplayName = "Lobby-Logout"),
	eGAMEPLAYING,	//UMETA(DisplayName = "GamePlaying"),
	eOK_VERIFICATION,//UMETA(DisplayName = "OK_VERIFICATION"),
	eNO_VERIFICATION,//UMETA(DisplayName = "FAILED_VERIFICATION"),
};

template<typename T>
std::string tostr(const T& value)
{
	std::ostringstream temp_str;
	temp_str << value;
	return temp_str.str();
}

template<typename T, typename...Args>
std::string tostr(const T& value, const Args&...args)
{
	return to_str(value) + to_str(args...);
}

struct tGUID : public GUID
{
	friend std::ostream& operator<<(std::ostream& stream, const tGUID& tGuid)
	{
		stream << tGuid.Data1;
		stream << tGuid.Data2;
		stream << tGuid.Data3;
		stream << tGuid.Data4[0];
		stream << tGuid.Data4[1];
		stream << tGuid.Data4[2];
		stream << tGuid.Data4[3];
		stream << tGuid.Data4[4];
		stream << tGuid.Data4[5];
		stream << tGuid.Data4[6];
		stream << tGuid.Data4[7];
		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, tGUID& tGuid)
	{
		stream >> tGuid.Data1;
		stream >> tGuid.Data2;
		stream >> tGuid.Data3;
		stream >> tGuid.Data4[0];
		stream >> tGuid.Data4[1];
		stream >> tGuid.Data4[2];
		stream >> tGuid.Data4[3];
		stream >> tGuid.Data4[4];
		stream >> tGuid.Data4[5];
		stream >> tGuid.Data4[6];
		stream >> tGuid.Data4[7];
		return stream;
	}
	/*friend std::ostream& operator<<(std::ostream& out, const tGUID& h)
	{
		int iSize = sizeof(tGUID);
		out.write(reinterpret_cast<const char*>(&h), iSize);
		return out;
	}*/

	/*friend std::istream& operator>>(std::istream& in, tGUID& h)
	{
		in.read(reinterpret_cast<char*>(&h), sizeof(tGUID));
		return in;
	}*/
};

struct GUIDComparer
{
	bool operator()(const tGUID& left, const tGUID& right) const
	{
		return (memcmp(&left, &right, sizeof(right)) < 0);
	}
};

#pragma pack(push, 1)
typedef struct
{
	WORD	iotype;	// ��Ŷ ����� ����
	WORD	type;	// � ��Ŷ�̴�.
	WORD	len;	// msg ����Ʈ ũ��
	DWORD	time;	// ��Ŷ ���� �ð�
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;

#define PACKET_HEADER_SIZE 10

typedef struct
{
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction;	 //0 ~7 8����
}TPACKET_USER_POSITION;

typedef struct
{
	int		iID;
	int		iType;
	char	name[20];
}TPACKET_CREATE_CHARACTER_INFO;

typedef struct
{
	tGUID	tGuid;
	FVector	vPosition;
	FVector	vVelocity;
	FRotator vRotation;
	float	fHealth;
	int		iType;
	bool	bAlive;
	bool	bAttack;
	bool	bDamage;
}TPACKET_CHARACTER_INFO;

typedef struct
{
	int		iType;
	FVector	vPosition;
}TPACKET_TARGETPOINT_INFO;

#pragma pack(pop)

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000,			// ��ȭ�� �Է¿�û	
	PACKET_CHAT_NAME_ACK,					// ��ȭ�� �Է� ����
	PACKET_CHAT_MSG,						// ���� ä�� �޼���.
	PACKET_LOGIN_REQ,						// �α��ο�û
	PACKET_LOGIN_ACK,						// �α�������
	PACKET_LOGOUT_REQ,						// �α׾ƿ���û
	PACKET_LOGOUT_ACK,						// �α׾ƿ�����
	PACKET_LOGOUT_PLAYER,					// �α׾ƿ� �÷��̾�

	PACKET_CREATE_ACCOUNT_REQ = 2000,		// ����������û
	PACKET_CREATE_ACCOUNT_ACK,				// ������������
	PACKET_DELETE_ACCOUNT_REQ,				// ����������û
	PACKET_DELETE_ACCOUNT_ACK,				// ������������

	PACKET_USER_POSITION = 3000,			// ĳ���� ��ġ �̵� ����
	PACKET_STOP_POSITION,					// ĳ���� ����
	PACKET_LOBBY_USER_INFO,					// ���� ��������Ʈ ����

	PACKET_CREATE_CHARACTER_REQ = 4000,		// ĳ���� ���� ��û
	PACKET_CREATE_CHARACTER_ACK,			// ĳ���� ���� ����
	PACKET_DELETE_CHARACTER_REQ,			// ĳ���� ���� ��û
	PACKET_DELETE_CHARACTER_ACK,			// ĳ���� ���� ����

	PACKET_HIT_CHARACTER = 5000,			// ĳ���� �ǰ� ����
	PACKET_HIT_MONSTER,						// ���� �ǰ� ����
	PACKET_ATTACK_CHARACTER,				// ĳ���� ���� ����
	PACKET_ATTACK_MONSTER,					// ���� ���� ����
	PACKET_DEAD_CHARACTER,					// ĳ���� ����
	PACKET_DEAD_MONSTER,					// ���� ����
	PACKET_DAMAGE_CHARACTER,				// ĳ���� ������ ����
	PACKET_DAMAGE_MONSTER,					// ���� ������ ����
	PACKET_SPAWN_CHARACTER,					// ĳ���� ����
	PACKET_SPAWN_MONSTER,					// ���� ����
	PACKET_SYNC_CHRACTER,					// ĳ���� ����ȭ
	PACKET_SYNC_MONSTER,					// ���� ����ȭ	
	PACKET_DESTROY_MONSTER,					// NPC �Ҹ�
	PACKET_NPC_TARGETMOVE,					// NPC Ÿ���̵�	

	PACKET_ZONE_ENTRY_REQ = 6000,			// ������ ���� ��û
	PACKET_ZONE_ENTRY_ACK,					// ���� �� ���� ����
	PACKET_ZONE_NEW_PLAYER,					// ���� �� ���� ĳ���� ���� ����
	PACKET_ZONE_PLAYER_INFO,				// ���� ���� ������ �������� �÷��� ĳ���� ���� ���� 
	PAKCET_ZONE_USER_INFO,					// ������ ��������Ʈ ����(�÷��̾�+��Ÿ(������ ��))
	PACKET_ZONE_TARGET_REQ,					// �� Ÿ�� ��ġ ��û  
	PACKET_ZONE_TARGET_ACK,					// �� Ÿ�� ��ġ ����
};

template<typename T>
std::istream& tBlockRead(std::istream& stream, T& value)
{
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
template<typename T>
std::ostream& tBlockWrite(std::ostream& stream, T& value)
{
	return stream.write(reinterpret_cast<char*>(&value), sizeof(T));
}

static std::ostream& operator<<(std::ostream& stream, const TPACKET_CHARACTER_INFO& tChar)
{
	stream.write(reinterpret_cast<const char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	return stream;
}

static std::istream& operator>>(std::istream& stream, TPACKET_CHARACTER_INFO& retChar)
{
	TPACKET_CHARACTER_INFO tChar;
	stream.read(reinterpret_cast<char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	retChar = std::move(tChar);
	return stream;
}

static std::ostream& operator<<(std::ostream& stream, const PACKET_HEADER& ph)
{
	stream.write(reinterpret_cast<const char*>(&ph), sizeof(PACKET_HEADER));
	return stream;
}

static std::istream& operator>>(std::istream& stream, PACKET_HEADER& ph)
{
	PACKET_HEADER value;

	stream.read(reinterpret_cast<char*>(&value), sizeof(PACKET_HEADER));
	ph = std::move(value);
	return stream;
}

static std::ostream& operator<<(std::ostream& stream, const UPACKET& packet)
{
	stream.write(reinterpret_cast<const char*>(&packet.ph), sizeof(PACKET_HEADER));
	stream.write(reinterpret_cast<const char*>(&packet.msg), packet.ph.len - sizeof(PACKET_HEADER));
	return stream;
}

static std::istream& operator>>(std::istream& stream, UPACKET& packet)
{
	UPACKET uPacket;
	stream.read(reinterpret_cast<char*>(&uPacket.ph), sizeof(PACKET_HEADER));
	stream.read(reinterpret_cast<char*>(&uPacket.msg), uPacket.ph.len - sizeof(PACKET_HEADER));
	packet = std::move(uPacket);
	return stream;
}

static std::stringstream MakePacket(int iType, stringstream& data)
{
	stringstream SendStream;
	PACKET_HEADER header;
	header.iotype = 777;
	header.type = iType;
	header.len = (WORD)(sizeof(PACKET_HEADER) + data.str().length());
	//header.time = ����ð�

	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}

