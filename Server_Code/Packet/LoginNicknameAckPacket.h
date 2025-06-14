#pragma once
#include "LoginPacket.h"


// �α��� �г��� �ߺ� Ȯ�� ����� Ŭ���̾�Ʈ���� �����ϴ� ���� ��Ŷ
// �ߺ� ���� + UID ���� (�ߺ��� ��� UID�� ����)
class LoginNicknameAckPacket : public LoginPacket
{
private:
    bool isDuplicate;
    uint32_t uid;

public:
    LoginNicknameAckPacket(bool isDuplicate = false, uint32_t uid = 0);

    bool GetIsDuplicate() const;
    void SetIsDuplicate(bool flag);

    uint32_t GetUID() const;
    void SetUID(uint32_t id);

    std::vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;

    ~LoginNicknameAckPacket() override = default;
};