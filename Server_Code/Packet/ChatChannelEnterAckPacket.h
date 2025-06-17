#pragma once
#include "ChatPacket.h"

// 채팅 채널 입장 응답 패킷
// 서버 → 클라이언트에게 입장 결과를 알려줄 때 사용합니다.
class ChatChannelEnterAckPacket : public ChatPacket
{
public:
    ChatChannelEnterAckPacket();
    ChatChannelEnterAckPacket(uint32_t uid, uint64_t timestamp);

    virtual std::vector<char> Serialize() const override;
    virtual void Deserialize(const char* buffer, size_t bufferSize) override;
    virtual bool IsValid(const char* buffer) const override;
};

