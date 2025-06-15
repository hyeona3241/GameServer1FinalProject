// �α��� ���� ��Ŷ���� ���� ���̽� Ŭ���� (�߻� Ŭ����)
// �г��� ������ �����ϸ�, ��ȿ�� �˻縦 ����.
#pragma once

#include <string>
#include "Packet.h"

class LoginPacket : public Packet
{
protected:
    string nickname;

public:
    explicit LoginPacket(EPacketType type, const string& nickname = "");
    
    // �г��� ������
    const string& GetNickname() const;
    void SetNickname(const string& nick);

    // �г��� ��ȿ�� �˻�
    virtual bool IsValid() const;
    // �־��� ���۰� ��ȿ���� Ȯ��
    bool IsValid(const char* buffer) const override;
    
    virtual ~LoginPacket() = default;
};

