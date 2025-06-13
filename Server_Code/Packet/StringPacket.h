// 문자열 데이터를 전송하기 위한 기본 패킷.
// 내부에 단일 문자열을 포함하며, 직렬화/역직렬화 기능 제공.

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

#include "BinaryWriter.h"
#include "BinaryReader.h"

#include "Packet.h"


// 예외 처리한 거 ErrorPacket에 추가해아함
// 아마 채팅 기본 베이스 클래스로 사용 될 확률 높음
class StringPacket : public Packet
{
protected:
    string data;

public:
    // 암묵적 변환을 금지
    explicit StringPacket(EPacketType type, const string& str = "");

    // 현재 패킷에 저장된 문자열 데이터를 반환
    inline const string& GetData() const { return data; }
    // 새로운 문자열 데이터를 설정
    inline void SetData(const string& newData)
    {
        if (newData.size() > 1024) {
            throw std::length_error("StringPacket data exceeds maximum size (1024 bytes)");
        }

        data = newData;
        header.size = static_cast<uint16_t>(sizeof(PacketHeader) + data.length() + 1);
    }
    
    vector<char> Serialize() const override;
    void Deserialize(const char* buffer, size_t bufferSize) override;
    bool IsValid(const char* buffer) const override;

    // 디버그용
    string ToString() const;

    virtual ~StringPacket() = default;

private:
    // 문자열 최대 길이 (컴파일 타임 상수)
    static constexpr size_t MAX_STRING_SIZE = 1024;
};

