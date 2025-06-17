#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include "Packet.h"

// ��Ʈ��ũ ������ ���� ���̳ʸ� ��Ŷ�� �����ϴ� Ŭ�����Դϴ�.
// �پ��� Ÿ���� ���������� ���ۿ� ����� �� �ֽ��ϴ�.
class BinaryWriter {
private:
    std::vector<char> buffer;

public:
    BinaryWriter() {
        buffer.reserve(1024);
    }

    void WriteUInt8(uint8_t value);
    void WriteUInt16(uint16_t value);
    void WriteUInt32(uint32_t value);
    void WriteUInt64(uint64_t value);
    void WriteInt32(int32_t value);
    void WriteFloat(float value);
    void WriteString(const std::string& str); // null-terminated
    void WriteHeader(const PacketHeader& header);
    void WriteHeader(EPacketType type, uint16_t size);

    const std::vector<char>& GetBuffer() const { return buffer; }
    size_t GetSize() const { return buffer.size(); }
};
