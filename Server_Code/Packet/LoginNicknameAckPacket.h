#pragma once
#include "LoginPacket.h"


// 로그인 닉네임 중복 확인 결과를 클라이언트에게 전송하는 응답 패킷
// 중복 여부 + UID 포함 (중복일 경우 UID는 생략)
class LoginNicknameAckPacket : public LoginPacket
{
private:
    bool isDuplicate;
    uint32_t uid;

public:
    LoginNicknameAckPacket(bool isDuplicate = false, uint32_t uid = 0);

    bool GetIsDuplicate() const;
    void SetIsDuplicate(bool flag);

    uint32_t GetUID() const;
    void SetUID(uint32_t id);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    ~LoginNicknameAckPacket() override = default;
};