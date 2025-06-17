#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>
#include "Packet.h"

// 수신된 바이너리 데이터를 읽기 위한 클래스입니다.
// 패킷 데이터를 각 타입별로 추출할 수 있도록 지원합니다.
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
    uint64_t ReadUInt64();
    int32_t ReadInt32();
    float ReadFloat();
    std::string ReadString(); // null-terminated 문자열
    struct PacketHeader ReadHeader();

    bool EndOfBuffer() const { return offset >= size; }

    inline const char* GetBuffer() const { return buffer; }
    inline size_t GetBufferSize() const { return size; }
};