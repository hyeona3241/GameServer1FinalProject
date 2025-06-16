#pragma once
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

