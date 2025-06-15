#include "BinaryReader.h"
#include <iostream>

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
    //�����: ���� �б� ��ġ�� ���� ����Ʈ �� Ȯ��
    std::cout << "[ReadString] offset = " << offset << ", buffer size = " << size << std::endl;
    std::cout << "[ReadString] peek bytes at offset: "
        << std::hex << (int)(unsigned char)buffer[offset] << " "
        << (int)(unsigned char)buffer[offset + 1] << std::dec << std::endl;

    // �ּ� 2����Ʈ�� �־�� ���ڿ� ����(uint16_t)�� ���� �� ����
    if (offset + sizeof(uint16_t) > size)
        throw std::out_of_range("ReadString: Not enough bytes for length");

    // ���ڿ� ���� �б�
    uint16_t len = 0;
    std::memcpy(&len, buffer + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    // ����� �α�: ����
    std::cout << "[ReadString] Length: " << len << std::endl;

    // ���� �ʰ� Ȯ��
    if (offset + len > size)
        throw std::out_of_range("ReadString: String length exceeds buffer size");

    // ����� �α�: ����Ʈ �� ���
    std::cout << "[ReadString] Raw bytes: ";
    for (int i = 0; i < len; ++i)
        std::cout << std::hex << (int)(unsigned char)buffer[offset + i] << " ";
    std::cout << std::dec << std::endl;

    // ���ڿ� ����
    std::string result(reinterpret_cast<const char*>(buffer + offset), len);

    // ����� �α�: ���� ���ڿ�
    std::cout << "[ReadString] Result: \"" << result << "\"" << std::endl;

    offset += len;
    return result;
}




PacketHeader BinaryReader::ReadHeader()
{
    EPacketType type = static_cast<EPacketType>(ReadUInt16());
    uint16_t packetSize = ReadUInt16();
    return PacketHeader(type, packetSize);
}
