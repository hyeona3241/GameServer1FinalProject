#pragma once
#include "LoginPacket.h"

// 클라이언트의 모델 선택 요청에 대한 서버 응답 패킷
// 선택된 모델 ID와 유효성 여부를 전달
class LoginModelSelectAckPacket : public LoginPacket
{
private:
    bool isValidModel;
    uint8_t modelId;

public:
    LoginModelSelectAckPacket();
    LoginModelSelectAckPacket(bool isValidModel, uint8_t modelId);

    bool GetResult() const { return isValidModel; }
    uint8_t GetModelId() const { return modelId; }

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    bool IsValid(const char* buffer, size_t size) const;

    ~LoginModelSelectAckPacket() override = default;
};

