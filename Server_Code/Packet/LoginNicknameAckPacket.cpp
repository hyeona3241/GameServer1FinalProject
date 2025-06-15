#include "LoginNicknameAckPacket.h"

LoginNicknameAckPacket::LoginNicknameAckPacket(bool isDuplicate, uint32_t uid)
    : LoginPacket(EPacketType::LOGIN_NICKNAME_ACK), isDuplicate(isDuplicate), uid(uid)
{
   
    // ��Ŷ ũ�� ���: UID�� �г��� �ߺ��� �ƴ� ���� ���Ե�
    header.size = sizeof(PacketHeader) + sizeof(uint8_t) + (isDuplicate ? 0 : sizeof(uint32_t));
}

bool LoginNicknameAckPacket::GetIsDuplicate() const { return isDuplicate; }
void LoginNicknameAckPacket::SetIsDuplicate(bool flag) { isDuplicate = flag; }

uint32_t LoginNicknameAckPacket::GetUID() const { return uid; }
void LoginNicknameAckPacket::SetUID(uint32_t id) { uid = id; }

std::vector<char> LoginNicknameAckPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt8(static_cast<uint8_t>(isDuplicate));

    // �ߺ��� �ƴ� ��쿡�� UID ����
    if (!isDuplicate) {
        writer.WriteUInt32(uid);
    }

    return writer.GetBuffer();
}

void LoginNicknameAckPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    // ���� ó�� : ���۰� �ּ� ũ�⺸�� ���� ���
    if (!buffer || bufferSize < sizeof(PacketHeader) + sizeof(uint8_t)) {
        throw std::runtime_error("Invalid buffer for LoginNicknameAckPacket.");
    }

    BinaryReader reader(buffer, bufferSize);

    header = reader.ReadHeader();

    // ���� ó��: header�� ��ϵ� size�� ���� size�� �ٸ��� ���� �Ǵ� �ջ�
    if (header.size != bufferSize) {
        throw std::runtime_error("Packet size mismatch.");
    }

    isDuplicate = static_cast<bool>(reader.ReadUInt8());

    if (!isDuplicate) {
        // ���� ó�� : �ߺ��� �ƴѵ� UID�� �����Ǿ��� ���
        if (bufferSize < sizeof(PacketHeader) + sizeof(uint8_t) + sizeof(uint32_t)) {
            throw std::runtime_error("Buffer too small for UID.");
        }

        uid = reader.ReadUInt32();

    }
    else {
        uid = 0; // �ߺ��� ��� UID ��ȿ
    }
}
