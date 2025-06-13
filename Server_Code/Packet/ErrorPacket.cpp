#include "ErrorPacket.h"

ErrorPacket::ErrorPacket(EErrorCode code, const std::string& message)
    : Packet(EPacketType::ERROR_MSG, 0), code(code), message(message)
{
    if (message.length() > MAX_ERROR_MSG_SIZE) {
        throw std::length_error("Error message exceeds maximum allowed size.");
    }

    // 패킷 전체 크기 계산 (헤더 + 에러 코드 + 메시지 + 널)
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

    // 유효성 검사
    if (message.empty() || message.length() > MAX_ERROR_MSG_SIZE)
        throw std::runtime_error("Malformed error message data.");
}
