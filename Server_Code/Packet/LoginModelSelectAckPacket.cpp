#include "LoginModelSelectAckPacket.h"

// 기본 생성자 - 역직렬화 시 사용
LoginModelSelectAckPacket::LoginModelSelectAckPacket()
    : LoginPacket(EPacketType::LOGIN_MODEL_SELECT_ACK),
    isValidModel(false), modelId(0)
{
}

// 유효성 결과와 모델 ID를 인자로 받는 생성자 - 서버가 응답 패킷 생성 시 사용
LoginModelSelectAckPacket::LoginModelSelectAckPacket(bool isValid, uint8_t modelId)
    : LoginPacket(EPacketType::LOGIN_MODEL_SELECT_ACK),
    isValidModel(isValid), modelId(modelId)
{
    if (modelId > MAX_MODEL_ID && isValid) {
        // 예외 처리 : 유효하지 않는 모델 ID
        throw std::invalid_argument("Invalid model ID: exceeds MAX_MODEL_ID");
    }
}

std::vector<char> LoginModelSelectAckPacket::Serialize() const {
    BinaryWriter writer;

    // 패킷 전체 크기: Header(4) + isValid(1) + modelId(1) = 6
    PacketHeader h = header;
    h.size = sizeof(PacketHeader) + sizeof(uint8_t) + sizeof(uint8_t);

    writer.WriteHeader(h);
    writer.WriteUInt8(static_cast<uint8_t>(isValidModel));
    writer.WriteUInt8(modelId);

    return writer.GetBuffer();
}


void LoginModelSelectAckPacket::Deserialize(const char* buffer, size_t bufferSize) {
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();

    if (header.type != EPacketType::LOGIN_MODEL_SELECT_ACK)
        // 예외 처리 : 패킷 구조 불일치
        throw std::runtime_error("Unexpected packet type in LoginModelSelectAckPacket");

    isValidModel = static_cast<bool>(reader.ReadUInt8());
    modelId = reader.ReadUInt8();

    if (modelId > MAX_MODEL_ID && isValidModel) {
        // 예외 처리 : 수신한 패킷이 isValidModel = true인데, modelId가 허용 범위를 벗어난 경우
        throw std::runtime_error("Invalid model ID received in ACK packet");
    }
}

bool LoginModelSelectAckPacket::IsValid(const char* buffer, size_t size) const {
    return buffer != nullptr && size >= sizeof(PacketHeader) + 2;
}
