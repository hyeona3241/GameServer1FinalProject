#pragma once
#include "LoginPacket.h"

// LOGIN_SUCCESS_ACK
// ������ �α��� ������ Ŭ���̾�Ʈ���� �˸��� UID�� �����ϴ� ��Ŷ
class LoginSuccessAckPacket : public LoginPacket
{
private:
    uint32_t uid;

public:
    LoginSuccessAckPacket(uint32_t uid, const std::string& nickname);
    LoginSuccessAckPacket();

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    inline uint32_t GetUID() const { return uid; }
    inline void SetUID(uint32_t id) { uid = id; }

};

