#pragma once
#include "ChatPacket.h"

// ä�� �޽��� ���� ��û ��Ŷ
// Ŭ���̾�Ʈ �� ������ ����ڰ� �Է��� �޽����� �����ϴ�.
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

