// ChatChannelEnterReqPacket.cpp
#include "ChatChannelEnterReqPacket.h"
#include <iostream>

ChatChannelEnterReqPacket::ChatChannelEnterReqPacket()
    : ChatChannelEnterReqPacket(0, 0) {
}

ChatChannelEnterReqPacket::ChatChannelEnterReqPacket(uint32_t uid, uint64_t timestamp)
    : ChatPacket(EPacketType::CHAT_CHANNEL_ENTER_REQ, uid, timestamp)
{
    header.size = sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint64_t);
}


std::vector<char> ChatChannelEnterReqPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    return writer.GetBuffer();
}

void ChatChannelEnterReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();

    std::cout << "[Deserialize] UID: " << uid << ", Timestamp: " << timestamp << std::endl;
}

bool ChatChannelEnterReqPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint64_t));
        PacketHeader h = reader.ReadHeader();
        if (static_cast<EPacketType>(h.type) != EPacketType::CHAT_CHANNEL_ENTER_REQ)
            return false;
        return true;
    }
    catch (...) {
        return false;
    }
}
