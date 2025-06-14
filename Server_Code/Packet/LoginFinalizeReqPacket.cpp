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
        // ���� ó�� : ���۰� null
        throw std::invalid_argument("LoginFinalizeReqPacket::IsValid: buffer is null");

    BinaryReader reader(buffer, sizeof(PacketHeader));
    PacketHeader h = reader.ReadHeader();

    if (h.type != EPacketType::LOGIN_FINALIZE_REQ)
        // ���� ó�� : ���� Ÿ�� ����ġ
        throw std::runtime_error("IsValid: Packet type mismatch");

    if (h.size != sizeof(PacketHeader))
        // ���� ó�� : ���� ������ ����ġ
        throw std::runtime_error("IsValid: Packet size mismatch");

    return true;
}