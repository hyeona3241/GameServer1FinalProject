#include "ChatBroadcastPacket.h"

ChatBroadcastPacket::ChatBroadcastPacket()
    :ChatBroadcastPacket(0, 0, "")
{
}

ChatBroadcastPacket::ChatBroadcastPacket(uint32_t uid, uint64_t timestamp, const std::string& msg)
    : ChatPacket(EPacketType::CHAT_BROADCAST, uid, timestamp), message(msg)
{
    header.size = sizeof(PacketHeader);
}

std::vector<char> ChatBroadcastPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    writer.WriteString(message);

    std::vector<char> buffer = writer.GetBuffer();
    *reinterpret_cast<uint16_t*>(&buffer[2]) = static_cast<uint16_t>(buffer.size());
    return buffer;
}

void ChatBroadcastPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();
    message = reader.ReadString();
}

bool ChatBroadcastPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader));
        PacketHeader h = reader.ReadHeader();
        return (static_cast<EPacketType>(h.type) == EPacketType::CHAT_BROADCAST);
    }
    catch (...) {
        return false;
    }
}