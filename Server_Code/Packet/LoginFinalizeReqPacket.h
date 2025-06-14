#pragma once
#include "LoginPacket.h"

// LOGIN_FINALIZE_REQ
// 최종 로그인을 서버에 요청하는 패킷.
class LoginFinalizeReqPacket : public LoginPacket
{
public:
    LoginFinalizeReqPacket();

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
 
    bool IsValid(const char* buffer) const override;
};

