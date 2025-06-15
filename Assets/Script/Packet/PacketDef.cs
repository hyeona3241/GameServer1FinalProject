using System;

// [1] 패킷 타입 정의
public enum EPacketType : ushort
{
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
}

// [2] 에러 코드 정의
public enum EErrorCode : ushort
{
    NONE = 0,

    INVALID_NICKNAME = 1,
    DUPLICATE_NICKNAME = 2,
    INVALID_MODEL_ID = 3,
    LOGIN_FAILED = 4,
    INVALID_UID = 5,

    INVALID_PACKET = 90,
    UNKNOWN_ERROR = 999
}

// [3] 공통 헤더 정의
public struct PacketHeader
{
    public EPacketType type;
    public ushort size;

    public PacketHeader(EPacketType type, ushort size)
    {
        this.type = type;
        this.size = size;
    }
}
