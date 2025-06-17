#pragma once
#include <stdexcept>
#include <iostream>

#include "Packet.h"


// 채팅 전용 공통 패킷 기반 클래스
// UID 및 타임스탬프를 포함하고, 직렬화/역직렬화 기본 기능을 제공합니다.
class ChatPacket : public Packet
{
protected:
    uint32_t uid;
    uint64_t timestamp;

public:
    ChatPacket(EPacketType type, uint32_t uid, uint64_t timestamp);

    inline void SetUID(uint32_t id) { uid = id; }
    inline void SetTimestamp(uint64_t time) { timestamp = time; }

    inline uint32_t GetUID() const { return uid; }
    inline uint64_t GetTimestamp() const { return timestamp; }

    // 기본 직렬화
    std::vector<char> SerializeBase() const;

    // 기본 역직렬화
    void DeserializeBase(const char* data, size_t size);
    
};

