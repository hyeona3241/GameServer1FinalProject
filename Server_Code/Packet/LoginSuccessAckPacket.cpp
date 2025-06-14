#include "LoginSuccessAckPacket.h"

#include <stdexcept>

LoginSuccessAckPacket::LoginSuccessAckPacket(uint32_t uid, const std::string& nickname)
    : LoginPacket(EPacketType::LOGIN_SUCCESS_ACK, nickname), uid(uid)
{
    header.size = static_cast<uint16_t>(
        sizeof(PacketHeader) + sizeof(uint32_t) + nickname.length() + 1
        );
}

LoginSuccessAckPacket::LoginSuccessAckPacket()
    : LoginPacket(EPacketType::LOGIN_SUCCESS_ACK, ""), uid(0)
{
}

std::vector<char> LoginSuccessAckPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt32(uid);
    writer.WriteString(nickname);
    return writer.GetBuffer();
}

void LoginSuccessAckPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    uid = reader.ReadUInt32();
    nickname = reader.ReadString();
}