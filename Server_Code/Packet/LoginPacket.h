// 로그인 관련 패킷들의 공통 베이스 클래스 (추상 클래스)
// 닉네임 정보를 포함하며, 유효성 검사를 수행.
#pragma once

#include <string>
#include "Packet.h"

class LoginPacket : public Packet
{
protected:
    string nickname;

public:
    explicit LoginPacket(EPacketType type, const string& nickname = "");
    
    // 닉네임 접근자
    const string& GetNickname() const;
    void SetNickname(const string& nick);

    // 닉네임 유효성 검사
    virtual bool IsValid() const;
    // 주어진 버퍼가 유효한지 확인
    bool IsValid(const char* buffer) const override;
    
    virtual ~LoginPacket() = default;
};

