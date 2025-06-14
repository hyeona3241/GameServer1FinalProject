#pragma once
#include "LoginPacket.h"

// LOGIN_MODEL_SELECT_REQ
// 클라이언트가 모델 선택 시 서버로 전송하는 요청 패킷
class LoginModelSelectReqPacket : public LoginPacket
{
private:
    uint8_t modelId;

public:
    LoginModelSelectReqPacket(uint8_t modelId = 0);

    inline uint8_t GetModelId() const { return modelId; }
    inline void SetModelId(uint8_t id) { modelId = id; }

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    ~LoginModelSelectReqPacket() override = default;
};
