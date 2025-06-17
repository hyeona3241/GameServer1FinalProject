#pragma once

// ChatManager 클래스
// ----------------------
// 이 클래스는 메인 서버에서 채팅 서버와의 연결을 관리하는 싱글톤 네트워크 매니저입니다.
// TCP 기반으로 채팅 서버와 연결되며, 패킷 송수신을 스레드로 처리합니다.
// 내부적으로 UID, 닉네임, 소켓 정보를 기반으로 접속 중인 유저 정보를 관리하며,
// 다음과 같은 기능을 제공합니다:
//
// - 채팅 서버 연결/해제 및 패킷 송수신
// - 채팅 유저 정보 등록, 조회, 삭제
// - UID 및 닉네임 기반 검색
// - 클라이언트 목록 관리 및 브로드캐스트 전송
// - 수신 루프 스레드를 통한 지속적인 패킷 처리

#include "ServerCommon.h"
#include "Packet.h"
#include <thread>
#include <atomic>
#include <mutex>

using ChatUserMap = std::unordered_map<uint32_t, ChatUserInfo>;     // UID -> Info
using ChatNicknameMap = std::unordered_map<std::string, uint32_t>;  // 닉네임 -> UID

class ChatManager
{
private:
    SOCKET chatSocket = INVALID_SOCKET;
    std::thread recvThread;
    std::atomic<bool> isReceiving = false;
    std::mutex sendMutex;

    WSADATA wsaData;

    ChatUserMap chatUserMap;
    ChatNicknameMap chatNicknameMap;
    mutable std::mutex userMutex;

public:
    static ChatManager& GetInstance();

    bool ConnectToChatServer(const std::string& ip, uint16_t port);
    void Disconnect();
    void SendPacket(const Packet& packet);

    void StartReceiveLoop(); // 패킷 수신 스레드 시작
    void StopReceiveLoop();  // 스레드 종료

    // 유저 관리
    void AddChatUser(uint32_t uid, const std::string& nickname, SOCKET socket);
    void RemoveChatUser(uint32_t uid);
    bool IsUserInChat(uint32_t uid) const;
    std::string GetNicknameByUID(uint32_t uid) const;
    uint32_t GetUIDByNickname(const std::string& nickname) const;
    SOCKET GetSocketByUID(uint32_t uid) const;
    std::vector<SOCKET> GetAllClientSockets() const;

    const std::unordered_map<uint32_t, ChatUserInfo>& GetAllChatUsers() const
    {
        return chatUserMap;
    }

private:
    ChatManager();
    ~ChatManager();

    void ReceiveLoop(); // 내부 수신 처리 루프
};