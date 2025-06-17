#include "LoginModelSelectAckPacket.h"

// �⺻ ������ - ������ȭ �� ���
LoginModelSelectAckPacket::LoginModelSelectAckPacket()
    : LoginPacket(EPacketType::LOGIN_MODEL_SELECT_ACK),
    isValidModel(false), modelId(0)
{
}

// ��ȿ�� ����� �� ID�� ���ڷ� �޴� ������ - ������ ���� ��Ŷ ���� �� ���
LoginModelSelectAckPacket::LoginModelSelectAckPacket(bool isValid, uint8_t modelId)
    : LoginPacket(EPacketType::LOGIN_MODEL_SELECT_ACK),
    isValidModel(isValid), modelId(modelId)
{
    if (modelId > MAX_MODEL_ID && isValid) {
        // ���� ó�� : ��ȿ���� �ʴ� �� ID
        throw std::invalid_argument("Invalid model ID: exceeds MAX_MODEL_ID");
    }
}

std::vector<char> LoginModelSelectAckPacket::Serialize() const {
    BinaryWriter writer;

    // ��Ŷ ��ü ũ��: Header(4) + isValid(1) + modelId(1) = 6
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
        // ���� ó�� : ��Ŷ ���� ����ġ
        throw std::runtime_error("Unexpected packet type in LoginModelSelectAckPacket");

    isValidModel = static_cast<bool>(reader.ReadUInt8());
    modelId = reader.ReadUInt8();

    if (modelId > MAX_MODEL_ID && isValidModel) {
        // ���� ó�� : ������ ��Ŷ�� isValidModel = true�ε�, modelId�� ��� ������ ��� ���
        throw std::runtime_error("Invalid model ID received in ACK packet");
    }
}

bool LoginModelSelectAckPacket::IsValid(const char* buffer, size_t size) const {
    return buffer != nullptr && size >= sizeof(PacketHeader) + 2;
}
