#pragma once

// ChatServer Ŭ����
// ------------------------
// �� Ŭ������ ä�� ������ �������� �Ǵ� �̱��� ���� Ŭ�����Դϴ�.
// ���� �����κ����� TCP ������ �����ϰ�, ��Ŷ ���� �� ������ ����մϴ�.
//
// �ֿ� ���:
// - ���� ���� �ʱ�ȭ �� ���ε�
// - ���� ������ ���� ���� �� �������� ���� ���� ����
// - ���ŵ� �����͸� BinaryReader�� �Ľ��ϰ� ChatPacketHandler�� ����
// - ��Ŷ �۽� �� ��Ŷ ���̿� �Բ� ����ȭ�� �����͸� ���� ������ ����

#include <thread>
#include "ServerCommon.h"

class ChatServer
{
private:
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET mainServerSocket = INVALID_SOCKET;
    std::thread recvThread;
    bool isRunning = false;

public:
    static ChatServer& GetInstance() {
        static ChatServer instance;
        return instance;
    }

    bool Start(const std::string& ip, uint16_t port);
    void Run();
    void Shutdown();
    bool SendPacket(const std::vector<char>& data);

private:
    ChatServer() = default;
    ~ChatServer() = default;

    ChatServer(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;

    void AcceptLoop();
    void ReceiveLoop();
};

