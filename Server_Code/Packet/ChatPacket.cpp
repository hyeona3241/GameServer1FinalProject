#include "ChatPacket.h"

ChatPacket::ChatPacket(EPacketType type, uint32_t uid, uint64_t timestamp)
	: Packet(type, 0), uid(uid), timestamp(timestamp)
{
}

std::vector<char> ChatPacket::SerializeBase() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    return writer.GetBuffer();
}

void ChatPacket::DeserializeBase(const char* data, size_t size)
{
    BinaryReader reader(data, size);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();
}
