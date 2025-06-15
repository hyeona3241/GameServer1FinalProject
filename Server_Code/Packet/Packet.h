#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "PacketDef.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"

using namespace std;


// 위치
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
    // 디버그용
    string ToString() const {
        return "Vector3(" + to_string(x) + "," + to_string(y) + "," + to_string(z) + ")";
    }
};

// 회전
struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f)
        : x(x), y(y), z(z), w(w) {
    }
    // 디버그용
    string ToString() const {
        return "Quaternion(" + to_string(x) + "," + to_string(y) + "," + to_string(z) + "," + to_string(w) + ")";
    }
};

// 네트워크 패킷의 기본 추상 클래스.
// 공통 헤더, 직렬화/역직렬화 인터페이스를 정의.
class Packet
{
protected:
    PacketHeader header;


public:
    Packet(PacketHeader header);
    Packet(EPacketType type, uint16_t size);

    // 패킷 타입을 반환
    inline EPacketType GetType() const { return header.type; }
    // 패킷 사이즈를 반환
    inline uint16_t GetHeaderSize() const { return header.size; }


    // 직렬화 : Packet -> 바이너리 버퍼
    virtual vector<char> Serialize() const = 0;
    // 역직렬화: 바이너리 버퍼 -> Packet
    virtual void Deserialize(const char* buffer, size_t bufferSize) = 0;
    
    // 주어진 버퍼가 유효한지 확인
    virtual bool IsValid(const char* buffer) const = 0;

    virtual ~Packet() = default;
};

