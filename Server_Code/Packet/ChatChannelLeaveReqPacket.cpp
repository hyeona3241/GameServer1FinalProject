#include "ChatChannelLeaveReqPacket.h"

ChatChannelLeaveReqPacket::ChatChannelLeaveReqPacket()
    : ChatChannelLeaveReqPacket(0, 0)
{
   
}

ChatChannelLeaveReqPacket::ChatChannelLeaveReqPacket(uint32_t uid, uint64_t timestamp)
    : ChatPacket(EPacketType::CHAT_CHANNEL_LEAVE_REQ, uid, timestamp)
{
    header.size = sizeof(PacketHeader) + sizeof(uid) + sizeof(timestamp);
}

std::vector<char> ChatChannelLeaveReqPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    return writer.GetBuffer();
}

void ChatChannelLeaveReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    if (bufferSize < sizeof(PacketHeader) + sizeof(uid) + sizeof(timestamp))
        throw std::runtime_error("Buffer too small for ChatChannelLeaveReqPacket");

    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();
}

bool ChatChannelLeaveReqPacket::IsValid(const char* buffer) const
{
    try
    {
        BinaryReader reader(buffer, sizeof(PacketHeader));
        PacketHeader h = reader.ReadHeader();

        return (static_cast<EPacketType>(h.type) == EPacketType::CHAT_CHANNEL_LEAVE_REQ);
    }
    catch (...)
    {
        return false;
    }
}
