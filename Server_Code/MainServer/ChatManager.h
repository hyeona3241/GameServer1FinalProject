#pragma once
#include "ServerCommon.h"
#include "Packet.h"
#include <thread>
#include <atomic>
#include <mutex>

using ChatUserMap = std::unordered_map<uint32_t, ChatUserInfo>;     // UID -> Info
using ChatNicknameMap = std::unordered_map<std::string, uint32_t>;  // 닉네임 -> UID

class ChatManager
{
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

    SOCKET chatSocket = INVALID_SOCKET;
    std::thread recvThread;
    std::atomic<bool> isReceiving = false;
    std::mutex sendMutex;
 
    WSADATA wsaData;

    ChatUserMap chatUserMap;
    ChatNicknameMap chatNicknameMap;
    mutable std::mutex userMutex;

};