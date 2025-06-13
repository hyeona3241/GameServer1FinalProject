#pragma once

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "LoginPacket.h"

// PacketType : LOGIN_NICKNAME_REQ
// �г��� �Է� �� ������ �����ϴ� ��û ��Ŷ
class LoginNicknameReqPacket :public LoginPacket
{
public:
    LoginNicknameReqPacket(const std::string& nickname);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    virtual ~LoginNicknameReqPacket() = default;
};

