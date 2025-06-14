#pragma once
#include "Packet.h"

// LOGOUT_REQ
// 클라이언트가 종료될 때 서버에게 알리는 패킷
class LogoutReqPacket : public Packet
{
public:
    LogoutReqPacket();

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

