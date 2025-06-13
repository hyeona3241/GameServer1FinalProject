#include "LoginNicknameReqPacket.h"

LoginNicknameReqPacket::LoginNicknameReqPacket(const std::string& nickname)
	: LoginPacket(EPacketType::LOGIN_NICKNAME_REQ, nickname)
{
}

std::vector<char> LoginNicknameReqPacket::Serialize() const
{
    BinaryWriter writer;

    writer.WriteHeader(header);
    writer.WriteString(nickname);

    return writer.GetBuffer();
}


void LoginNicknameReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);

    header = reader.ReadHeader();
    nickname = reader.ReadString();

    // 유효성 검사
    if (nickname.empty() || nickname.size() > 16)
    {
        throw std::runtime_error("Invalid nickname received in LoginNicknameReqPacket");
    }
}
