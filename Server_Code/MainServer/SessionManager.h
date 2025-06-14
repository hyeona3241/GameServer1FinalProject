#pragma once

// SessionManager 클래스
// ----------------------
// 이 클래스는 서버에 접속한 클라이언트들의 세션 정보를 관리하는 싱글톤 클래스입니다.
// 각 클라이언트는 고유한 UID, 닉네임, 소켓 정보를 가지며, 이를 기반으로 조회 및 중복 검사를 수행합니다.
// 스레드 안전성을 위해 std::mutex로 보호되며, 다음 기능을 제공합니다:
// - 유저 추가 및 제거
// - UID, 소켓, 닉네임 기반 조회
// - 닉네임 중복 검사
// - 전체 유저 정보 디버깅 출력

#include <mutex>
#include "ServerCommon.h"

class SessionManager
{
private:
    // UID → UserSession
    UserMap uidToUser;
    // 닉네임 → UID
    NicknameMap nicknameToUid;
    // 소켓 → UID
    std::unordered_map<SOCKET, uint32_t> socketToUid; 

    std::mutex sessionMutex;

private:
    SessionManager() = default;

public:
    static SessionManager& GetInstance();

    void AddUser(SOCKET socket, const UserSession& session);
    void RemoveUser(SOCKET socket);

    UserSession* GetUserBySocket(SOCKET socket);
    UserSession* GetUserByUID(uint32_t uid);
    bool IsNicknameDuplicate(const std::string& nickname);
    uint32_t RegisterNewUser(UserSession& session);
    bool SetModelId(uint32_t uid, uint8_t modelId);

    void PrintAllUsers(); // 디버깅용

};
