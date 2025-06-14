#pragma once
#include "LoginPacket.h"

// Ŭ���̾�Ʈ�� �� ���� ��û�� ���� ���� ���� ��Ŷ
// ���õ� �� ID�� ��ȿ�� ���θ� ����
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

