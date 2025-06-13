#pragma once

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "LoginPacket.h"

// PacketType : LOGIN_NICKNAME_REQ
// 닉네임 입력 후 서버에 전달하는 요청 패킷
class LoginNicknameReqPacket :public LoginPacket
{
public:
    LoginNicknameReqPacket(const std::string& nickname);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    virtual ~LoginNicknameReqPacket() = default;
};

