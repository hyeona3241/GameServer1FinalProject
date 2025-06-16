#pragma once
#pragma once

#include <cstdint>

// ��Ŷ Ÿ���� �����ϱ� ���� ������
enum class EPacketType : uint16_t {
    None = 0,

    // �α��� ��Ŷ : 1000~1999
    LOGIN_NICKNAME_REQ = 1001,
    LOGIN_NICKNAME_ACK = 1002,

    LOGIN_MODEL_SELECT_REQ = 1003,
    LOGIN_MODEL_SELECT_ACK = 1004,

    LOGIN_FINALIZE_REQ = 1005,
    LOGIN_SUCCESS_ACK = 1006,

    LOGOUT_REQ = 1007,


    // ä�� ��Ŷ: 2000~2999
    CHAT_CHANNEL_ENTER_REQ = 2001, // ä�� ���� ��û (Ŭ -> �� -> ä)
    // ���� ��û �Ҷ��� ���� �������� ��ȿ�� UID���� Ȯ��
    CHAT_CHANNEL_ENTER_ACK = 2002,  // (ä -> �� -> Ŭ )

    CHAT_SEND_REQ = 2003,   // (Ŭ -> �� -> ü)
    CHAT_BROADCAST = 2004,  // (ä -> �� -> Ŭ)

    CHAT_CHANNEL_LEAVE_REQ = 2005, // ä�� ���� ��û (Ŭ -> ��)

    CHAT_ERROR = 2099,  // ä�� ���� ���� ���� (��: ä�� ����, ���� ���� ��)


    // �ý��� : 9000~9999
    ERROR_MSG = 9001
};

// ���� ��Ŷ ��� ����ü
#pragma pack(push, 1)
struct PacketHeader
{
    EPacketType type;
    uint16_t size;

    PacketHeader() : type(EPacketType::None), size(0) {}
    inline PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};
#pragma pack(pop)

// �ִ� �� ID ����
constexpr uint8_t MAX_MODEL_ID = 3;

// ���� �ڵ� ����
enum class EErrorCode : uint16_t {
    NONE = 0,

    // �߸��� �г���
    INVALID_NICKNAME = 1,
    // �ߺ��� �г���
    DUPLICATE_NICKNAME = 2,
    // �߸��� �� ID
    INVALID_MODEL_ID = 3,
    // �α��� ����
    LOGIN_FAILED = 4,

    // �߸��� UID
    INVALID_UID = 5,

    //�ִ� �����ڼ�
    SERVER_FULL = 6,

    // �߸��� ��Ŷ ����
    INVALID_PACKET = 90,

    UNKNOWN_ERROR = 999
};