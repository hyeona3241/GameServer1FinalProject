#pragma once
#include "Packet.h"

// LOGOUT_REQ
// Ŭ���̾�Ʈ�� ����� �� �������� �˸��� ��Ŷ
class LogoutReqPacket : public Packet
{
public:
    LogoutReqPacket();

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

