#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "PacketDef.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"

using namespace std;


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

