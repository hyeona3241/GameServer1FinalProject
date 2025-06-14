// PacketType : ErrorPacket
// Ŭ���̾�Ʈ�� ���� �ڵ�� �޽����� �����ϴ� ��Ŷ.

#pragma once

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "Packet.h"

constexpr size_t MAX_ERROR_MSG_SIZE = 512;

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

    UNKNOWN_ERROR = 999
};

// ���� ó�� �� ���� �޽����� �����ϴ� ��Ŷ
class ErrorPacket : public Packet
{
private:
    EErrorCode code;
    string message;

public:
    ErrorPacket(EErrorCode code, const std::string& message = "");

    EErrorCode GetCode() const;
    const std::string& GetMessage() const;

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    bool IsValid(const char* buffer) const override;
};

