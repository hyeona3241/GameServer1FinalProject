#pragma once
#include "ChatPacket.h"

class ChatChannelEnterAckPacket : public ChatPacket
{
public:
    ChatChannelEnterAckPacket();
    ChatChannelEnterAckPacket(uint32_t uid, uint64_t timestamp);

    virtual std::vector<char> Serialize() const override;
    virtual void Deserialize(const char* buffer, size_t bufferSize) override;
    virtual bool IsValid(const char* buffer) const override;
};

