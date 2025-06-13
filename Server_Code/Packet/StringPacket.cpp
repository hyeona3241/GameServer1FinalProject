#include "StringPacket.h"

StringPacket::StringPacket(EPacketType type, const string& str)
    //��� ũ�� + ���ڿ� ���� + �� ����
    : Packet(type, static_cast<uint16_t>(sizeof(PacketHeader) + str.length() + 1)), data(str)
{
    if (str.size() > MAX_STRING_SIZE) {
        // ���� ó�� : ���ڿ� ���̰� ����(MAX_STRING_SIZE)�� �ʰ�
        throw std::length_error("StringPacket initialization: data exceeds maximum size.");
    }
}

vector<char> StringPacket::Serialize() const
{
    if (data.size() + sizeof(PacketHeader) + 1 > UINT16_MAX) {
        throw std::overflow_error("Packet too large to serialize.");
    }

    BinaryWriter writer;

    writer.WriteHeader(header);

    writer.WriteString(data);

    return writer.GetBuffer();
}


void StringPacket::Deserialize(const char* buffer, size_t bufferSize)
{
    if (!buffer || bufferSize < sizeof(PacketHeader)) {
        // ���� ó�� : ���۰� �ʹ� ���� ��� ���� �߻�
        throw std::runtime_error("Invalid buffer for StringPacket::Deserialize");
    }

    BinaryReader reader(buffer, bufferSize);

    header = reader.ReadHeader();

    data = reader.ReadString();

    if (data.size() > MAX_STRING_SIZE) {
        // ���� ó�� : �߸��� �������� ��� ���� �߻�
        throw std::runtime_error("Deserialized string exceeds allowed size.");
    }
}



bool StringPacket::IsValid(const char* buffer) const
{
	if (buffer == nullptr) return false;
	return true;
}

string StringPacket::ToString() const
{
    // ��Ŷ ���� ���
    return "[PacketType: " + std::to_string(static_cast<uint16_t>(header.type)) +
        ", Size: " + std::to_string(header.size) +
        ", Data: \"" + data + "\"]";
}
