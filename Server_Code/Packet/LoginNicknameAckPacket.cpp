#include "LoginNicknameAckPacket.h"

LoginNicknameAckPacket::LoginNicknameAckPacket(bool isDuplicate, uint32_t uid)
    : LoginPacket(EPacketType::LOGIN_NICKNAME_ACK), isDuplicate(isDuplicate), uid(uid)
{
   
    // 패킷 크기 계산: UID는 닉네임 중복이 아닐 때만 포함됨
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

    // 중복이 아닐 경우에만 UID 포함
    if (!isDuplicate) {
        writer.WriteUInt32(uid);
    }

    return writer.GetBuffer();
}

void LoginNicknameAckPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    // 예외 처리 : 버퍼가 최소 크기보다 작을 경우
    if (!buffer || bufferSize < sizeof(PacketHeader) + sizeof(uint8_t)) {
        throw std::runtime_error("Invalid buffer for LoginNicknameAckPacket.");
    }

    BinaryReader reader(buffer, bufferSize);

    header = reader.ReadHeader();

    // 예외 처리: header에 기록된 size와 실제 size가 다르면 조작 또는 손상
    if (header.size != bufferSize) {
        throw std::runtime_error("Packet size mismatch.");
    }

    isDuplicate = static_cast<bool>(reader.ReadUInt8());

    if (!isDuplicate) {
        // 예외 처리 : 중복이 아닌데 UID가 누락되었을 경우
        if (bufferSize < sizeof(PacketHeader) + sizeof(uint8_t) + sizeof(uint32_t)) {
            throw std::runtime_error("Buffer too small for UID.");
        }

        uid = reader.ReadUInt32();

    }
    else {
        uid = 0; // 중복인 경우 UID 무효
    }
}
