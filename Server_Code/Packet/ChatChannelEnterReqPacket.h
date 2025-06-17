#pragma once
#include "ChatPacket.h"


// ä�� ä�� ���� ��û ��Ŷ
// Ŭ���̾�Ʈ �� ������ ä�� ������ �õ��� �� �����մϴ�.
class ChatChannelEnterReqPacket : public ChatPacket
{
public:
    ChatChannelEnterReqPacket();  // �⺻ ������
    ChatChannelEnterReqPacket(uint32_t uid, uint64_t timestamp); // UID, �ð� ���� ������

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;
};

