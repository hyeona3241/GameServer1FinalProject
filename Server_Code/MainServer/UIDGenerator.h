#pragma once

// UIDGenerator Ŭ����
// ----------------------
// �� Ŭ������ �������� ����� ������ UID�� ���������� �����ϴ� �̱��� ��ƿ��Ƽ Ŭ�����Դϴ�.
// ������ �����ϰ� UID�� �����ϱ� ���� std::mutex�� ����ϸ�, ���� ����� �����մϴ�:
// - 1000���� �����ϴ� UID �ڵ� ����
// - UID �ߺ� ���� ������ �� ����
// - �̱��� �������� ���� �ν��Ͻ� ����

#include <mutex>


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
    UIDGenerator(); 
    UIDGenerator(const UIDGenerator&) = delete;
    UIDGenerator& operator=(const UIDGenerator&) = delete;

};
