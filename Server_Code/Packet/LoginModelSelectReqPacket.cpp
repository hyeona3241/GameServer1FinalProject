#include "LoginModelSelectReqPacket.h"

LoginModelSelectReqPacket::LoginModelSelectReqPacket(uint8_t modelId)
    :LoginPacket(EPacketType::LOGIN_MODEL_SELECT_REQ), modelId(modelId)
{
}

std::vector<char> LoginModelSelectReqPacket::Serialize() const
{
    BinaryWriter writer;
    writer.WriteHeader(header);
    writer.WriteUInt8(modelId);
    return writer.GetBuffer();
}

void LoginModelSelectReqPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    BinaryReader reader(buffer, bufferSize);
    header = reader.ReadHeader();
    modelId = reader.ReadUInt8();

    if (modelId > MAX_MODEL_ID) {
        throw std::invalid_argument("Invalid model ID selected");
    }
}
