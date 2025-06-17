#pragma once
#include "ChatPacket.h"

// 채팅 채널 퇴장 요청 패킷
// 클라이언트 → 서버로 채팅 퇴장을 알릴 때 사용합니다.
class ChatChannelLeaveReqPacket : public ChatPacket
{
public:
    ChatChannelLeaveReqPacket();  // 기본 생성자
    ChatChannelLeaveReqPacket(uint32_t uid, uint64_t timestamp); // UID, 시간 포함 생성자

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

