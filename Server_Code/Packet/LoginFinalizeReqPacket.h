#pragma once
#include "LoginPacket.h"

// LOGIN_FINALIZE_REQ
// ���� �α����� ������ ��û�ϴ� ��Ŷ.
class LoginFinalizeReqPacket : public LoginPacket
{
public:
    LoginFinalizeReqPacket();

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
 
    bool IsValid(const char* buffer) const override;
};

