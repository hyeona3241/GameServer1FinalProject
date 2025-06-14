#pragma once
#include <mutex>

// UID�� ���������� �߱��ϴ� ��ƿ��Ƽ Ŭ����
class UIDGenerator
{
private:
    std::mutex uidMutex;
    uint32_t currentUID;

public:
    static UIDGenerator& GetInstance();

    // ���ο� UID�� ��ȯ
    uint32_t GenerateUID();

private:
    UIDGenerator();  // ������ �����
    UIDGenerator(const UIDGenerator&) = delete;
    UIDGenerator& operator=(const UIDGenerator&) = delete;

};
