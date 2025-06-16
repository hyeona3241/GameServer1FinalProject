#pragma once
#pragma once

#include <cstdint>

// 패킷 타입을 구분하기 위한 열거형
enum class EPacketType : uint16_t {
    None = 0,

    // 로그인 패킷 : 1000~1999
    LOGIN_NICKNAME_REQ = 1001,
    LOGIN_NICKNAME_ACK = 1002,

    LOGIN_MODEL_SELECT_REQ = 1003,
    LOGIN_MODEL_SELECT_ACK = 1004,

    LOGIN_FINALIZE_REQ = 1005,
    LOGIN_SUCCESS_ACK = 1006,

    LOGOUT_REQ = 1007,


    // 채팅 패킷: 2000~2999
    CHAT_CHANNEL_ENTER_REQ = 2001, // 채널 입장 요청 (클 -> 메 -> 채)
    // 입장 요청 할때만 메인 서버에서 유효한 UID인지 확인
    CHAT_CHANNEL_ENTER_ACK = 2002,  // (채 -> 메 -> 클 )

    CHAT_SEND_REQ = 2003,   // (클 -> 메 -> 체)
    CHAT_BROADCAST = 2004,  // (채 -> 메 -> 클)

    CHAT_CHANNEL_LEAVE_REQ = 2005, // 채널 퇴장 요청 (클 -> 메)

    CHAT_ERROR = 2099,  // 채팅 전용 오류 응답 (예: 채널 없음, 권한 없음 등)


    // 시스템 : 9000~9999
    ERROR_MSG = 9001
};

// 공통 패킷 헤더 구조체
#pragma pack(push, 1)
struct PacketHeader
{
    EPacketType type;
    uint16_t size;

    PacketHeader() : type(EPacketType::None), size(0) {}
    inline PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};
#pragma pack(pop)

// 최대 모델 ID 정의
constexpr uint8_t MAX_MODEL_ID = 3;

// 에러 코드 정의
enum class EErrorCode : uint16_t {
    NONE = 0,

    // 잘못된 닉네임
    INVALID_NICKNAME = 1,
    // 중복된 닉네임
    DUPLICATE_NICKNAME = 2,
    // 잘못된 모델 ID
    INVALID_MODEL_ID = 3,
    // 로그인 실패
    LOGIN_FAILED = 4,

    // 잘못된 UID
    INVALID_UID = 5,

    //최대 접속자수
    SERVER_FULL = 6,

    // 잘못된 패킷 정보
    INVALID_PACKET = 90,

    UNKNOWN_ERROR = 999
};