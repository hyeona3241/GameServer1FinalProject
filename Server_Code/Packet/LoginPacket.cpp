#include "LoginPacket.h"
#include <stdexcept>

LoginPacket::LoginPacket(EPacketType type, const string& nickname)
	: Packet(type, 0), nickname(nickname) 
{
	if (!IsValid()) {
		// 예외 처리 : 닉네임은 1~16자로 입력
		throw std::invalid_argument("Invalid nickname: must be 1~16 characters.");
	}
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
