#include "ChatSendReqPacket.h"

ChatSendReqPacket::ChatSendReqPacket()
    :ChatSendReqPacket(0, 0, "") {
}

ChatSendReqPacket::ChatSendReqPacket(uint32_t uid, uint64_t timestamp, const std::string& msg)
    : ChatPacket(EPacketType::CHAT_SEND_REQ, uid, timestamp), message(msg)
{
    header.size = sizeof(PacketHeader); // 이후 Serialize에서 갱신됨
}

std::vector<char> ChatSendReqPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteUInt64(timestamp);
    writer.WriteString(message);

    std::vector<char> buffer = writer.GetBuffer();
    // WriteHeader의 사이즈가 Serialize 전에 고정이므로 여기서 정확히 갱신
    uint16_t totalSize = static_cast<uint16_t>(buffer.size());
    *reinterpret_cast<uint16_t*>(&buffer[2]) = totalSize; // size 필드 위치는 offset 2

    return buffer;
}

void ChatSendReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    timestamp = reader.ReadUInt64();
    message = reader.ReadString();
}

bool ChatSendReqPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader));
        PacketHeader h = reader.ReadHeader();
        return (static_cast<EPacketType>(h.type) == EPacketType::CHAT_SEND_REQ);
    }
    catch (...) {
        return false;
    }
}
