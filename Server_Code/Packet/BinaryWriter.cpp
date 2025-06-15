#include "BinaryWriter.h"

void BinaryWriter::WriteUInt8(uint8_t value)
{
    buffer.push_back(static_cast<char>(value));
}

void BinaryWriter::WriteUInt16(uint16_t value) {
    char bytes[2];
    std::memcpy(bytes, &value, sizeof(uint16_t));
    buffer.insert(buffer.end(), bytes, bytes + sizeof(uint16_t));
}

void BinaryWriter::WriteUInt32(uint32_t value)
{
    char bytes[4];
    std::memcpy(bytes, &value, sizeof(uint32_t));
    buffer.insert(buffer.end(), bytes, bytes + sizeof(uint32_t));
}

void BinaryWriter::WriteInt32(int32_t value) {
    char bytes[4];
    std::memcpy(bytes, &value, sizeof(int32_t));
    buffer.insert(buffer.end(), bytes, bytes + sizeof(int32_t));
}

void BinaryWriter::WriteFloat(float value) {
    char bytes[4];
    std::memcpy(bytes, &value, sizeof(float));
    buffer.insert(buffer.end(), bytes, bytes + sizeof(float));
}

void BinaryWriter::WriteString(const std::string& str) {
    uint16_t len = static_cast<uint16_t>(str.length());
    WriteUInt16(len); // 문자열 길이 먼저 기록
    buffer.insert(buffer.end(), str.begin(), str.end());
}

void BinaryWriter::WriteHeader(const PacketHeader& header) {
    WriteUInt16(static_cast<uint16_t>(header.type));
    WriteUInt16(header.size);
}

void BinaryWriter::WriteHeader(EPacketType type, uint16_t size) {
    WriteHeader(PacketHeader(type, size));
}