#pragma once

// UIDGenerator 클래스
// ----------------------
// 이 클래스는 서버에서 사용할 고유한 UID를 순차적으로 생성하는 싱글톤 유틸리티 클래스입니다.
// 스레드 안전하게 UID를 생성하기 위해 std::mutex를 사용하며, 다음 기능을 제공합니다:
// - 1000부터 시작하는 UID 자동 증가
// - UID 중복 없이 유일한 값 보장
// - 싱글톤 패턴으로 단일 인스턴스 유지

#include <mutex>


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
    UIDGenerator(); 
    UIDGenerator(const UIDGenerator&) = delete;
    UIDGenerator& operator=(const UIDGenerator&) = delete;

};
