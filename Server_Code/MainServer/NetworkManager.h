#pragma once

// NetworkManager
// --------------------
// 서버의 네트워크 입출력을 총괄하는 싱글톤 클래스입니다.
// - 서버 소켓 초기화 및 클라이언트 연결 수락
// - 클라이언트별 수신 스레드 생성
// - 패킷 송수신 처리 및 종료 관리

#include <thread>
#include <mutex>
#include <atomic>

#include "ServerCommon.h"


class NetworkManager
{
private:
    SOCKET listenSocket = INVALID_SOCKET;            // 서버 리슨 소켓
    std::vector<std::thread> clientThreads;          // 클라이언트별 스레드
    std::mutex threadMutex;                          // 스레드 목록 보호용 뮤텍스
    std::atomic<bool> isRunning = false;             // 서버 실행 상태 플래그

private:
    NetworkManager() = default;
    ~NetworkManager() = default;

    // 연결 수락 루프
    void AcceptLoop();

    // 클라이언트별 수신 스레드
    void ClientWorker(SOCKET clientSocket);

public:
    static NetworkManager& GetInstance();

    // 서버 시작
    bool StartServer(uint16_t port);

    // 서버 종료
    void Shutdown();

    // 클라이언트에게 패킷 전송
    bool SendPacket(SOCKET clientSocket, const std::vector<char>& data);


};
