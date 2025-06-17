#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include "Packet.h"

// 네트워크 전송을 위한 바이너리 패킷을 구성하는 클래스입니다.
// 다양한 타입을 순차적으로 버퍼에 기록할 수 있습니다.
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
