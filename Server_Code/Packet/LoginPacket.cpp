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

bool LoginPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader));
        PacketHeader h = reader.ReadHeader();

        // 패킷 타입 검사 (이 클래스의 타입과 일치해야 함)
        if (h.type != this->header.type)
            throw std::runtime_error("Invalid packet type in buffer.");

        // 전체 버퍼로 다시 검사
        BinaryReader fullReader(buffer, h.size);
        fullReader.ReadHeader(); // 다시 헤더 읽기

        std::string nick = fullReader.ReadString();

        // 닉네임이 1~16자 이내인지 검사
        return !nick.empty() && nick.length() <= 16;
    }
    catch (...) {
        return false;
    }
}

