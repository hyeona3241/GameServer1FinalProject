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
    CHAT_SEND = 2001,
    CHAT_RECV = 2002,

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

    // 잘못된 패킷 정보
    INVALID_PACKET = 90,

    UNKNOWN_ERROR = 999
};