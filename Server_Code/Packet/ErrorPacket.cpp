#include "ErrorPacket.h"

ErrorPacket::ErrorPacket(EErrorCode code, const std::string& message)
    : Packet(EPacketType::ERROR_MSG, 0), code(code), message(message)
{
    if (message.length() > MAX_ERROR_MSG_SIZE) {
        throw std::length_error("Error message exceeds maximum allowed size.");
    }

    uint16_t byteLen = static_cast<uint16_t>(message.size());
    header.size = static_cast<uint16_t>(
        sizeof(PacketHeader) + sizeof(EErrorCode) + sizeof(uint16_t) + byteLen);

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

    // 유효성 검사
    if (message.empty() || message.length() > MAX_ERROR_MSG_SIZE)
        throw std::runtime_error("Malformed error message data.");
}

bool ErrorPacket::IsValid(const char* buffer) const {
    if (!buffer)
        throw std::invalid_argument("ErrorPacket::IsValid: buffer is null");

    BinaryReader reader(buffer, MAX_ERROR_MSG_SIZE + sizeof(PacketHeader) + sizeof(uint16_t) + 1);

    PacketHeader h = reader.ReadHeader();

    if (h.type != EPacketType::ERROR_MSG)
        throw std::runtime_error("IsValid: Packet type mismatch");

    // 메시지 검증
    const std::string msg = reader.ReadString();
    if (msg.empty())
        throw std::runtime_error("IsValid: Error message is empty");

    if (msg.size() > MAX_ERROR_MSG_SIZE)  // .size() = byte length for std::string
        throw std::runtime_error("IsValid: Error message exceeds max length");


    return true;
}
