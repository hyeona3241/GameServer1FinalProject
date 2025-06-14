#include "UIDGenerator.h"

UIDGenerator::UIDGenerator()
    : currentUID(1000) // ���� UID ��ȣ (1000���� ����)
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
