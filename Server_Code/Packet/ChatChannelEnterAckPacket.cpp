#include "ChatChannelEnterAckPacket.h"

ChatChannelEnterAckPacket::ChatChannelEnterAckPacket()
    : ChatChannelEnterAckPacket(0, 0) {
}

ChatChannelEnterAckPacket::ChatChannelEnterAckPacket(uint32_t uid, uint64_t timestamp)
    : ChatPacket(EPacketType::CHAT_CHANNEL_ENTER_ACK, uid, timestamp)
{
    header.size = sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint64_t);
}

std::vector<char> ChatChannelEnterAckPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    return writer.GetBuffer();
}

void ChatChannelEnterAckPacket::Deserialize(const char* buffer, size_t size)
{
    BinaryReader reader(buffer, size);
    PacketHeader h = reader.ReadHeader();
    if (static_cast<EPacketType>(h.type)!=EPacketType::CHAT_CHANNEL_ENTER_ACK)
        throw std::runtime_error("Invalid packet type for ChatChannelEnterAckPacket");

    header = h;
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();
}

bool ChatChannelEnterAckPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader) + sizeof(uint32_t) + sizeof(uint64_t));
        PacketHeader h = reader.ReadHeader();

        return static_cast<EPacketType>(h.type) == EPacketType::CHAT_CHANNEL_ENTER_ACK;
    }
    catch (...) {
        return false;
    }
}