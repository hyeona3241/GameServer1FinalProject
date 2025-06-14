#pragma once

// SessionManager Ŭ����
// ----------------------
// �� Ŭ������ ������ ������ Ŭ���̾�Ʈ���� ���� ������ �����ϴ� �̱��� Ŭ�����Դϴ�.
// �� Ŭ���̾�Ʈ�� ������ UID, �г���, ���� ������ ������, �̸� ������� ��ȸ �� �ߺ� �˻縦 �����մϴ�.
// ������ �������� ���� std::mutex�� ��ȣ�Ǹ�, ���� ����� �����մϴ�:
// - ���� �߰� �� ����
// - UID, ����, �г��� ��� ��ȸ
// - �г��� �ߺ� �˻�
// - ��ü ���� ���� ����� ���

#include <mutex>
#include "ServerCommon.h"

class SessionManager
{
private:
    // UID �� UserSession
    UserMap uidToUser;
    // �г��� �� UID
    NicknameMap nicknameToUid;
    // ���� �� UID
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

    void PrintAllUsers(); // ������

};
