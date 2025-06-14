#pragma once

// NetworkManager
// --------------------
// ������ ��Ʈ��ũ ������� �Ѱ��ϴ� �̱��� Ŭ�����Դϴ�.
// - ���� ���� �ʱ�ȭ �� Ŭ���̾�Ʈ ���� ����
// - Ŭ���̾�Ʈ�� ���� ������ ����
// - ��Ŷ �ۼ��� ó�� �� ���� ����

#include <thread>
#include <mutex>
#include <atomic>

#include "ServerCommon.h"


class NetworkManager
{
private:
    SOCKET listenSocket = INVALID_SOCKET;            // ���� ���� ����
    std::vector<std::thread> clientThreads;          // Ŭ���̾�Ʈ�� ������
    std::mutex threadMutex;                          // ������ ��� ��ȣ�� ���ؽ�
    std::atomic<bool> isRunning = false;             // ���� ���� ���� �÷���

private:
    NetworkManager() = default;
    ~NetworkManager() = default;

    // ���� ���� ����
    void AcceptLoop();

    // Ŭ���̾�Ʈ�� ���� ������
    void ClientWorker(SOCKET clientSocket);

public:
    static NetworkManager& GetInstance();

    // ���� ����
    bool StartServer(uint16_t port);

    // ���� ����
    void Shutdown();

    // Ŭ���̾�Ʈ���� ��Ŷ ����
    bool SendPacket(SOCKET clientSocket, const std::vector<char>& data);


};
