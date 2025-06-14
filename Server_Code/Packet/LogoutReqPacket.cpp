#include "LogoutReqPacket.h"

LogoutReqPacket::LogoutReqPacket()
    : Packet(EPacketType::LOGOUT_REQ, sizeof(PacketHeader))
{
}

std::vector<char> LogoutReqPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    return writer.GetBuffer();
}

void LogoutReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();

    if (header.type != EPacketType::LOGOUT_REQ)
        throw std::runtime_error("LogoutReqPacket::Deserialize - invalid packet type");

    if (header.size != sizeof(PacketHeader))
        throw std::runtime_error("LogoutReqPacket::Deserialize - invalid packet size");
}

bool LogoutReqPacket::IsValid(const char* buffer) const
{
    if (!buffer)
        throw std::invalid_argument("LogoutReqPacket::IsValid - buffer is null");

    BinaryReader reader(buffer, sizeof(PacketHeader));
    PacketHeader h = reader.ReadHeader();

    if (h.type != EPacketType::LOGOUT_REQ)
        throw std::runtime_error("LogoutReqPacket::IsValid - packet type mismatch");

    if (h.size != sizeof(PacketHeader))
        throw std::runtime_error("LogoutReqPacket::IsValid - packet size mismatch");

    return true;
}