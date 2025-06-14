#include "LoginFinalizeReqPacket.h"

LoginFinalizeReqPacket::LoginFinalizeReqPacket()
	: LoginPacket(EPacketType::LOGIN_FINALIZE_REQ)
{
}

std::vector<char> LoginFinalizeReqPacket::Serialize() const
{
	BinaryWriter writer;
	writer.WriteHeader(header);
	return writer.GetBuffer();
}

void LoginFinalizeReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
	BinaryReader reader(buffer, bufferSize);
	header = reader.ReadHeader();
}

bool LoginFinalizeReqPacket::IsValid(const char* buffer) const {
    if (!buffer)
        // 예외 처리 : 버퍼가 null
        throw std::invalid_argument("LoginFinalizeReqPacket::IsValid: buffer is null");

    BinaryReader reader(buffer, sizeof(PacketHeader));
    PacketHeader h = reader.ReadHeader();

    if (h.type != EPacketType::LOGIN_FINALIZE_REQ)
        // 예외 처리 : 버퍼 타입 불일치
        throw std::runtime_error("IsValid: Packet type mismatch");

    if (h.size != sizeof(PacketHeader))
        // 예외 처리 : 버퍼 사이즈 불일치
        throw std::runtime_error("IsValid: Packet size mismatch");

    return true;
}