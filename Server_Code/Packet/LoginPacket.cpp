#include "LoginPacket.h"
#include <stdexcept>

LoginPacket::LoginPacket(EPacketType type, const string& nickname)
	: Packet(type, 0), nickname(nickname) 
{
	if (!IsValid()) {
		// ���� ó�� : �г����� 1~16�ڷ� �Է�
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
		// ���� ó�� : �г����� 1~16�ڷ� �Է�
		throw std::invalid_argument("Nickname must be between 1 and 16 characters.");
	}
	nickname = nick;
}

bool LoginPacket::IsValid() const
{
	// 16�� ������ ���
	return !nickname.empty() && nickname.length() <= 16;
}
