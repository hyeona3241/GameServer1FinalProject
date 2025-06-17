#pragma once
#include "ChatPacket.h"


// 채팅 채널 입장 요청 패킷
// 클라이언트 → 서버로 채널 접속을 시도할 때 전송합니다.
class ChatChannelEnterReqPacket : public ChatPacket
{
public:
    ChatChannelEnterReqPacket();  // 기본 생성자
    ChatChannelEnterReqPacket(uint32_t uid, uint64_t timestamp); // UID, 시간 포함 생성자

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

