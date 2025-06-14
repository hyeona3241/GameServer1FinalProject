#include "UIDGenerator.h"

UIDGenerator::UIDGenerator()
    : currentUID(1000) // 시작 UID 번호 (1000부터 시작)
{
}

UIDGenerator& UIDGenerator::GetInstance()
{
    static UIDGenerator instance;
    return instance;
}

uint32_t UIDGenerator::GenerateUID()
{
    std::lock_guard<std::mutex> lock(uidMutex);
    return currentUID++;
}
