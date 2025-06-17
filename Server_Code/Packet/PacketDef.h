#pragma once

#include <cstdint>

//
// PacketDef.h
// └ 클라이언트-서버 간의 통신에서 사용되는 패킷 타입과 공통 헤더, 에러 코드를 정의한 헤더 파일입니다.
//    모든 패킷 구조체/클래스에서 이 파일을 포함하여 타입을 구분하고 기본 정보를 설정합니다.
//

// ------------------------------------------------------------
//    패킷 타입 열거형
//    각 패킷을 구분하기 위한 고유 식별자 역할을 합니다.
//    범위별로 기능 구분 (로그인, 채팅, 시스템)
// ------------------------------------------------------------
enum class EPacketType : uint16_t {
    None = 0,

    // 로그인 관련 패킷: 1000 ~ 1999
    LOGIN_NICKNAME_REQ = 1001,   // 닉네임 등록 요청
    LOGIN_NICKNAME_ACK = 1002,   // 닉네임 등록 응답

    LOGIN_MODEL_SELECT_REQ = 1003, // 모델 선택 요청
    LOGIN_MODEL_SELECT_ACK = 1004, // 모델 선택 응답

    LOGIN_FINALIZE_REQ = 1005,     // 로그인 최종 확정 요청
    LOGIN_SUCCESS_ACK = 1006,      // 로그인 완료 응답

    LOGOUT_REQ = 1007,             // 로그아웃 요청

    // 채팅 관련 패킷: 2000 ~ 2999
    CHAT_CHANNEL_ENTER_REQ = 2001,  // 채팅 채널 입장 요청 (클라이언트 → 메인 → 채팅 서버)
    CHAT_CHANNEL_ENTER_ACK = 2002,  // 채팅 채널 입장 응답 (채팅 서버 → 메인 → 클라이언트)

    CHAT_SEND_REQ = 2003,           // 채팅 메시지 전송 요청 (클라이언트 → 메인 → 채팅)
    CHAT_BROADCAST = 2004,          // 채팅 메시지 브로드캐스트 (채팅 서버 → 메인 → 클라이언트)

    CHAT_CHANNEL_LEAVE_REQ = 2005,  // 채팅 채널 퇴장 요청 (클라이언트 → 메인)

    // 시스템 관련 패킷: 9000 ~ 9999
    ERROR_MSG = 9001               // 에러 메시지 전송 패킷
};

// ------------------------------------------------------------
//    PacketHeader 구조체
//    모든 패킷의 앞부분에 붙는 공통 헤더입니다.
//    패킷 타입과 전체 크기를 포함하여 구분 및 파싱을 도와줍니다.
// ------------------------------------------------------------
#pragma pack(push, 1)
struct PacketHeader
{
    EPacketType type;   // 패킷 타입 (EPacketType)
    uint16_t size;      // 전체 패킷 크기 (Header + Body)

    PacketHeader() : type(EPacketType::None), size(0) {}
    PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};
#pragma pack(pop)

// ------------------------------------------------------------
//    최대 모델 ID 상수
// ------------------------------------------------------------
constexpr uint8_t MAX_MODEL_ID = 3;

// ------------------------------------------------------------
//    에러 코드 열거형
//    서버 → 클라이언트로 전달되는 오류 상태를 정의합니다.
// ------------------------------------------------------------
enum class EErrorCode : uint16_t {
    NONE = 0,                  // 에러 없음

    INVALID_NICKNAME = 1,     // 닉네임 형식 오류
    DUPLICATE_NICKNAME = 2,   // 중복된 닉네임
    INVALID_MODEL_ID = 3,     // 존재하지 않는 모델 ID
    LOGIN_FAILED = 4,         // 로그인 실패
    INVALID_UID = 5,          // 유효하지 않은 UID
    SERVER_FULL = 6,          // 최대 접속자 수 초과

    INVALID_PACKET = 90,      // 포맷이 잘못된 패킷

    CHAT_ERROR = 201,         // 채팅 처리 중 에러

    UNKNOWN_ERROR = 999       // 알 수 없는 에러
};
