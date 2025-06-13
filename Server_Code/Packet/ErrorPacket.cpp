#include "ErrorPacket.h"

ErrorPacket::ErrorPacket(EErrorCode code, const std::string& message)
    : Packet(EPacketType::ERROR_MSG, 0), code(code), message(message)
{
    if (message.length() > MAX_ERROR_MSG_SIZE) {
        throw std::length_error("Error message exceeds maximum allowed size.");
    }

    // ��Ŷ ��ü ũ�� ��� (��� + ���� �ڵ� + �޽��� + ��)
    header.size = static_cast<uint16_t>(
        sizeof(PacketHeader) + sizeof(EErrorCode) + message.length() + 1);
}

EErrorCode ErrorPacket::GetCode() const
{
    return code;
}

const std::string& ErrorPacket::GetMessage() const
{
    return message;
}

std::vector<char> ErrorPacket::Serialize() const
{
    BinaryWriter writer;
    
    writer.WriteHeader(header);

    writer.WriteUInt16(static_cast<uint16_t>(code));

    writer.WriteString(message);

    return writer.GetBuffer();
}


void ErrorPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);

    header = reader.ReadHeader();

    code = static_cast<EErrorCode>(reader.ReadUInt16());

    message = reader.ReadString();

    // ��ȿ�� �˻�
    if (message.empty() || message.length() > MAX_ERROR_MSG_SIZE)
        throw std::runtime_error("Malformed error message data.");
}
