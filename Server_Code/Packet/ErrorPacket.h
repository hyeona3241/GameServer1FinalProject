// PacketType : ErrorPacket
// 클라이언트에 에러 코드와 메시지를 전달하는 패킷.

#pragma once

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "Packet.h"

constexpr size_t MAX_ERROR_MSG_SIZE = 512;

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

    UNKNOWN_ERROR = 999
};

// 예외 처리 된 에러 메시지를 전달하는 패킷
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

