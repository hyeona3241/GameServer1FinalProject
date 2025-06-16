#include "SessionManager.h"
#include "UIDGenerator.h"

SessionManager& SessionManager::GetInstance()
{
    static SessionManager instance;
    return instance;
}

// 유저 세션을 등록하고 관련 맵에 정보 추가
void SessionManager::AddUser(SOCKET socket, const UserSession& session)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    uidToUser[session.uid] = session;
    nicknameToUid[session.nickname] = session.uid;
    socketToUid[socket] = session.uid;
}

// 접속 종료된 유저 정보 모두 제거 (비정상 종료)
void SessionManager::RemoveUser(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    auto sockIt = socketToUid.find(socket);
    if (sockIt != socketToUid.end()) {
        uint32_t uid = sockIt->second;
        auto userIt = uidToUser.find(uid);
        if (userIt != uidToUser.end()) {
            nicknameToUid.erase(userIt->second.nickname);
            uidToUser.erase(userIt);
        }
        socketToUid.erase(sockIt);
    }
}

// 접속 종료된 유저 정보 모두 제거
bool SessionManager::RemoveUser(uint32_t uid)
{
    std::lock_guard<std::mutex> lock(sessionMutex);

    auto it = uidToUser.find(uid);
    if (it != uidToUser.end()) {
        const std::string& nickname = it->second.nickname;
        SOCKET sock = it->second.socket;

        nicknameToUid.erase(nickname);
        uidToUser.erase(it);
        socketToUid.erase(sock); // socketToUid도 정리해줘야 함

        return true;
    }
    return false; // uid가 존재하지 않음
}

// 소켓으로 UID를 조회하고 해당 유저 정보 반환
UserSession* SessionManager::GetUserBySocket(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    auto it = socketToUid.find(socket);
    if (it == socketToUid.end()) return nullptr;

    auto userIt = uidToUser.find(it->second);
    if (userIt != uidToUser.end()) return &userIt->second;

    return nullptr;
}

// UID로 유저 정보를 조회
UserSession* SessionManager::GetUserByUID(uint32_t uid)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    auto it = uidToUser.find(uid);
    if (it != uidToUser.end()) return &it->second;
    return nullptr;
}

// 닉네임 중복 검사
bool SessionManager::IsNicknameDuplicate(const std::string& nickname)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    return nicknameToUid.find(nickname) != nicknameToUid.end();
}

// UID 부여
uint32_t SessionManager::RegisterNewUser(UserSession& session)
{
    std::lock_guard<std::mutex> lock(sessionMutex);

    uint32_t uid = UIDGenerator::GetInstance().GenerateUID();
    session.uid = uid;

    uidToUser[uid] = session;
    nicknameToUid[session.nickname] = uid;

    return uid;
}


bool SessionManager::SetModelId(uint32_t uid, uint8_t modelId)
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    auto it = uidToUser.find(uid);
    if (it == uidToUser.end())
        return false;

    it->second.modelId = modelId;
    return true;
}

size_t SessionManager::GetUserCount()
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    return uidToUser.size();
}


// 모든 유저 정보를 출력 (디버깅용)
void SessionManager::PrintAllUsers()
{
    std::lock_guard<std::mutex> lock(sessionMutex);
    for (const auto& pair : uidToUser)
    {
        const uint32_t& uid = pair.first;
        const UserSession& session = pair.second;
        std::cout << session.ToString() << std::endl;
    }
}
