#include "LoginNicknameReqPacket.h"

LoginNicknameReqPacket::LoginNicknameReqPacket()
    : LoginPacket(EPacketType::LOGIN_NICKNAME_REQ)
{
}

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

    //header = reader.ReadHeader();
    nickname = reader.ReadString();

 
    std::cout << "[Deserialize] Nickname: '" << nickname << "' (" << nickname.length() << " chars)\n";


    if (nickname.empty() || nickname.length() > 16 || nickname.size() > 255)
        throw std::runtime_error("Invalid nickname in LoginNicknameReqPacket");
}

