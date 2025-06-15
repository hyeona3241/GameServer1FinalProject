#pragma once
#include "Packet.h"

// LOGOUT_REQ
// 클라이언트가 종료될 때 서버에게 알리는 패킷
class LogoutReqPacket : public Packet
{
private:
    uint32_t uid = 0;

public:
    LogoutReqPacket();
    LogoutReqPacket(uint32_t uid);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;

    inline uint32_t GetUID() const { return uid; }
    inline void SetUID(uint32_t id) { uid = id; }
};

