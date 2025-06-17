#pragma once
#include "ChatPacket.h"

// 채팅 메시지 전송 요청 패킷
// 클라이언트 → 서버로 사용자가 입력한 메시지를 보냅니다.
class ChatSendReqPacket : public ChatPacket
{
private:
    std::string message;

public:
    ChatSendReqPacket();
    ChatSendReqPacket(uint32_t uid, uint64_t timestamp, const std::string& msg);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;

    inline const std::string& GetMessage() const { return message; }
    inline void SetMessage(const std::string& msg) { message = msg; }

};

