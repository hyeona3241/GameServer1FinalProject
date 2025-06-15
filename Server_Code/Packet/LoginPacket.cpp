#include "LoginPacket.h"
#include <stdexcept>

LoginPacket::LoginPacket(EPacketType type, const string& nickname)
	: Packet(type, 0), nickname(nickname) 
{
}

const string& LoginPacket::GetNickname() const
{
	return nickname;
}

void LoginPacket::SetNickname(const string& nick)
{
	if (nick.empty() || nick.length() > 16) {
		// 예외 처리 : 닉네임은 1~16자로 입력
		throw std::invalid_argument("Nickname must be between 1 and 16 characters.");
	}
	nickname = nick;
}

bool LoginPacket::IsValid() const
{
	// 16자 까지만 허용
	return !nickname.empty() && nickname.length() <= 16;
}

bool LoginPacket::IsValid(const char* buffer) const
{
	try {
		BinaryReader reader(buffer, sizeof(PacketHeader));
		PacketHeader h = reader.ReadHeader();

		// 공통 패킷 타입 확인
		return (h.type == this->header.type);
	}
	catch (...) {
		return false;
	}
}


