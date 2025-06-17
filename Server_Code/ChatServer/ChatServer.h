#pragma once

// ChatServer 클래스
// ------------------------
// 이 클래스는 채팅 서버의 진입점이 되는 싱글톤 서버 클래스입니다.
// 메인 서버로부터의 TCP 연결을 수락하고, 패킷 수신 및 전송을 담당합니다.
//
// 주요 기능:
// - 서버 소켓 초기화 및 바인딩
// - 메인 서버의 연결 수락 및 지속적인 수신 루프 실행
// - 수신된 데이터를 BinaryReader로 파싱하고 ChatPacketHandler에 위임
// - 패킷 송신 시 패킷 길이와 함께 직렬화된 데이터를 메인 서버로 전송

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

