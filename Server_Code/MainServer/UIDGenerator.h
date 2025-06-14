#pragma once
#include <mutex>

// UID를 순차적으로 발급하는 유틸리티 클래스
class UIDGenerator
{
private:
    std::mutex uidMutex;
    uint32_t currentUID;

public:
    static UIDGenerator& GetInstance();

    // 새로운 UID를 반환
    uint32_t GenerateUID();

private:
    UIDGenerator();  // 생성자 비공개
    UIDGenerator(const UIDGenerator&) = delete;
    UIDGenerator& operator=(const UIDGenerator&) = delete;

};
