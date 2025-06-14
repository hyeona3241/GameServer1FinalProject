#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "BinaryWriter.h"
#include "BinaryReader.h"

using namespace std;

enum class EPacketType : uint16_t {
    None = 0,

    // �α��� ��Ŷ : 1000~1999
    LOGIN_NICKNAME_REQ = 1001, 
    LOGIN_NICKNAME_ACK = 1002, 
    LOGIN_MODEL_SELECT_REQ = 1003,
    LOGIN_MODEL_SELECT_ACK = 1004, 
    LOGIN_FINALIZE_REQ = 1005,
    LOGIN_SUCCESS_ACK = 1006,
    LOGOUT_REQ = 1007,

    // ä�� ��Ŷ: 2000~2999
    CHAT_SEND = 2001,
    CHAT_RECV = 2002,

    // �ý��� : 9000~9999
    ERROR_MSG = 9001
};

struct PacketHeader
{
    EPacketType type;
    uint16_t size;

    PacketHeader() : type(EPacketType::None), size(0) {}
    inline PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};

// ��ġ
struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float x = 0.f, float y = 0.f, float z = 0.f)
        : x(x), y(y), z(z) {
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    // ����׿�
    string ToString() const {
        return "Vector3(" + to_string(x) + "," + to_string(y) + "," + to_string(z) + ")";
    }
};

// ȸ��
struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f)
        : x(x), y(y), z(z), w(w) {
    }
    // ����׿�
    string ToString() const {
        return "Quaternion(" + to_string(x) + "," + to_string(y) + "," + to_string(z) + "," + to_string(w) + ")";
    }
};


// ��Ʈ��ũ ��Ŷ�� �⺻ �߻� Ŭ����.
// ���� ���, ����ȭ/������ȭ �������̽��� ����.
class Packet
{
protected:
    PacketHeader header;


public:
    Packet(PacketHeader header);
    Packet(EPacketType type, uint16_t size);

    // ��Ŷ Ÿ���� ��ȯ
    inline EPacketType GetType() const { return header.type; }
    // ��Ŷ ����� ��ȯ
    inline uint16_t GetHeaderSize() const { return header.size; }


    // ����ȭ : Packet -> ���̳ʸ� ����
    virtual vector<char> Serialize() const = 0;
    // ������ȭ: ���̳ʸ� ���� -> Packet
    virtual void Deserialize(const char* buffer, size_t bufferSize) = 0;
    
    // �־��� ���۰� ��ȿ���� Ȯ��
    virtual bool IsValid(const char* buffer) const = 0;

    virtual ~Packet() = default;
};

