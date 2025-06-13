#include "BinaryReader.h"

BinaryReader::BinaryReader(const char* buffer, size_t size)
    : buffer(buffer), size(size), offset(0)
{
    if (!buffer || size == 0)
        throw std::invalid_argument("BinaryReader: Buffer is null or size is zero.");
}

uint8_t BinaryReader::ReadUInt8()
{
    if (offset + sizeof(uint8_t) > size)
        throw std::out_of_range("BinaryReader: Buffer overrun on ReadUInt8");

    uint8_t value = static_cast<uint8_t>(buffer[offset]);
    offset += sizeof(uint8_t);
    return value;
}

uint16_t BinaryReader::ReadUInt16()
{
    if (offset + sizeof(uint16_t) > size)
        throw std::out_of_range("BinaryReader: Buffer overrun on ReadUInt16");

    uint16_t value;
    std::memcpy(&value, buffer + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    return value;
}

uint32_t BinaryReader::ReadUInt32()
{
    if (offset + sizeof(uint32_t) > size)
        throw std::out_of_range("BinaryReader: Buffer overrun on ReadUInt32");

    uint32_t value;
    std::memcpy(&value, buffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    return value;
}

int32_t BinaryReader::ReadInt32()
{
    if (offset + sizeof(int32_t) > size)
        throw std::out_of_range("BinaryReader: Buffer overrun on ReadInt32");

    int32_t value;
    std::memcpy(&value, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    return value;
}

float BinaryReader::ReadFloat()
{
    if (offset + sizeof(float) > size)
        throw std::out_of_range("BinaryReader: Buffer overrun on ReadFloat");

    float value;
    std::memcpy(&value, buffer + offset, sizeof(float));
    offset += sizeof(float);
    return value;
}

std::string BinaryReader::ReadString()
{
    size_t start = offset;

    while (offset < size && buffer[offset] != '\0') {
        ++offset;
    }

    if (offset >= size)
        throw std::out_of_range("BinaryReader: String not null-terminated");

    std::string result(buffer + start, offset - start);
    ++offset; // null ¹®ÀÚ °Ç³Ê¶Ü
    return result;
}

PacketHeader BinaryReader::ReadHeader()
{
    EPacketType type = static_cast<EPacketType>(ReadUInt16());
    uint16_t packetSize = ReadUInt16();
    return PacketHeader(type, packetSize);
}
