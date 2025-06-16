#pragma once
#include "ChatPacket.h"

class ChatChannelLeaveReqPacket : public ChatPacket
{
public:
    ChatChannelLeaveReqPacket();  // �⺻ ������
    ChatChannelLeaveReqPacket(uint32_t uid, uint64_t timestamp); // UID, �ð� ���� ������

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

