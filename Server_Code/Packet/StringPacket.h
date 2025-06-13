// ���ڿ� �����͸� �����ϱ� ���� �⺻ ��Ŷ.
// ���ο� ���� ���ڿ��� �����ϸ�, ����ȭ/������ȭ ��� ����.

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "Packet.h"


// ���� ó���� �� ErrorPacket�� �߰��ؾ���
// �Ƹ� ä�� �⺻ ���̽� Ŭ������ ��� �� Ȯ�� ����
class StringPacket : public Packet
{
protected:
    string data;

public:
    // �Ϲ��� ��ȯ�� ����
    explicit StringPacket(EPacketType type, const string& str = "");

    // ���� ��Ŷ�� ����� ���ڿ� �����͸� ��ȯ
    inline const string& GetData() const { return data; }
    // ���ο� ���ڿ� �����͸� ����
    inline void SetData(const string& newData)
    {
        if (newData.size() > 1024) {
            throw std::length_error("StringPacket data exceeds maximum size (1024 bytes)");
        }

        data = newData;
        header.size = static_cast<uint16_t>(sizeof(PacketHeader) + data.length() + 1);
    }
    
    vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;

    // ����׿�
    string ToString() const;

    virtual ~StringPacket() = default;

private:
    // ���ڿ� �ִ� ���� (������ Ÿ�� ���)
    static constexpr size_t MAX_STRING_SIZE = 1024;
};

