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
	WORD	iotype;	// 패킷 입출력 종류
	WORD	type;	// 어떤 패킷이다.
	WORD	len;	// msg 바이트 크기
	DWORD	time;	// 패킷 전송 시간
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
	WORD	direction;	 //0 ~7 8방향
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

	PACKET_CHAT_NAME_REQ = 1000,			// 대화명 입력요청	
	PACKET_CHAT_NAME_ACK,					// 대화명 입력 응답
	PACKET_CHAT_MSG,						// 보통 채팅 메세지.
	PACKET_LOGIN_REQ,						// 로그인요청
	PACKET_LOGIN_ACK,						// 로그인응답
	PACKET_LOGOUT_REQ,						// 로그아웃요청
	PACKET_LOGOUT_ACK,						// 로그아웃응답
	PACKET_LOGOUT_PLAYER,					// 로그아웃 플레이어

	PACKET_CREATE_ACCOUNT_REQ = 2000,		// 계정생성요청
	PACKET_CREATE_ACCOUNT_ACK,				// 계정생성응답
	PACKET_DELETE_ACCOUNT_REQ,				// 계정삭제요청
	PACKET_DELETE_ACCOUNT_ACK,				// 계정삭제응답

	PACKET_USER_POSITION = 3000,			// 캐릭터 위치 이동 시작
	PACKET_STOP_POSITION,					// 캐릭터 정지
	PACKET_LOBBY_USER_INFO,					// 대기방 유저리스트 정보

	PACKET_CREATE_CHARACTER_REQ = 4000,		// 캐릭터 생성 요청
	PACKET_CREATE_CHARACTER_ACK,			// 캐릭터 생성 응답
	PACKET_DELETE_CHARACTER_REQ,			// 캐릭터 삭제 요청
	PACKET_DELETE_CHARACTER_ACK,			// 캐릭터 삭제 응답

	PACKET_HIT_CHARACTER = 5000,			// 캐릭터 피격 받음
	PACKET_HIT_MONSTER,						// 몬스터 피격 받음
	PACKET_ATTACK_CHARACTER,				// 캐릭터 공격 시작
	PACKET_ATTACK_MONSTER,					// 몬스터 공격 시작
	PACKET_DEAD_CHARACTER,					// 캐릭터 죽음
	PACKET_DEAD_MONSTER,					// 몬스터 죽음
	PACKET_DAMAGE_CHARACTER,				// 캐릭터 데미지 받음
	PACKET_DAMAGE_MONSTER,					// 몬스터 데미지 받음
	PACKET_SPAWN_CHARACTER,					// 캐릭터 스폰
	PACKET_SPAWN_MONSTER,					// 몬스터 스폰
	PACKET_SYNC_CHRACTER,					// 캐릭터 동기화
	PACKET_SYNC_MONSTER,					// 몬스터 동기화	
	PACKET_DESTROY_MONSTER,					// NPC 소멸
	PACKET_NPC_TARGETMOVE,					// NPC 타켓이동	

	PACKET_ZONE_ENTRY_REQ = 6000,			// 게임존 입장 요청
	PACKET_ZONE_ENTRY_ACK,					// 게임 존 입장 응답
	PACKET_ZONE_NEW_PLAYER,					// 게임 존 입장 캐릭터 정보 전송
	PACKET_ZONE_PLAYER_INFO,				// 게임 존에 입장한 유저들의 플레이 캐릭터 정보 전송 
	PAKCET_ZONE_USER_INFO,					// 게임존 유저리스트 정보(플레이어+기타(옵저버 등))
	PACKET_ZONE_TARGET_REQ,					// 존 타켓 위치 요청  
	PACKET_ZONE_TARGET_ACK,					// 존 타켓 위치 응답
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
	//header.time = 현재시간

	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}

