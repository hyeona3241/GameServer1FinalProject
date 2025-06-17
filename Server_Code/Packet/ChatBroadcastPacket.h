#pragma once
#include "ChatPacket.h"

// 채팅 메시지 브로드캐스트 패킷
// 서버 → 모든 클라이언트에게 메시지를 전달할 때 사용합니다.
class ChatBroadcastPacket : public ChatPacket
{
private:
    std::string message;

public:
    ChatBroadcastPacket();
    ChatBroadcastPacket(uint32_t uid, uint64_t timestamp, const std::string& msg);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;

    inline const std::string& GetMessage() const { return message; }
    inline void SetMessage(const std::string& msg) { message = msg; }
};

