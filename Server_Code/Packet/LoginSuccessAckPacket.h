#pragma once
#include "LoginPacket.h"

// LOGIN_SUCCESS_ACK
// 서버가 로그인 성공을 클라이언트에게 알리고 UID를 전달하는 패킷
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

