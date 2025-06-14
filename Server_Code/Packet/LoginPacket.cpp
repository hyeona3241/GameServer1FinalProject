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

bool LoginPacket::IsValid(const char* buffer) const
{
    try {
        BinaryReader reader(buffer, sizeof(PacketHeader));
        PacketHeader h = reader.ReadHeader();

        // ��Ŷ Ÿ�� �˻� (�� Ŭ������ Ÿ�԰� ��ġ�ؾ� ��)
        if (h.type != this->header.type)
            throw std::runtime_error("Invalid packet type in buffer.");

        // ��ü ���۷� �ٽ� �˻�
        BinaryReader fullReader(buffer, h.size);
        fullReader.ReadHeader(); // �ٽ� ��� �б�

        std::string nick = fullReader.ReadString();

        // �г����� 1~16�� �̳����� �˻�
        return !nick.empty() && nick.length() <= 16;
    }
    catch (...) {
        return false;
    }
}

