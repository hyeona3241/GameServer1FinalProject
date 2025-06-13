#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>
#include "Packet.h"

class BinaryReader {
private:
    const char* buffer;
    size_t size;
    size_t offset = 0;

public:
    BinaryReader(const char* buffer, size_t size);

    uint8_t ReadUInt8();
    uint16_t ReadUInt16();
    uint32_t ReadUInt32();
    int32_t ReadInt32();
    float ReadFloat();
    std::string ReadString(); // null-terminated ¹®ÀÚ¿­
    PacketHeader ReadHeader();

    bool EndOfBuffer() const { return offset >= size; }
};
