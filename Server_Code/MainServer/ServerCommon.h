#pragma once

// =========================
// ���� ���� ��� ���� ����
// - ���� ���� ����ü
// - ��Ŷ Ÿ�� ����
// - ���� ��Ʈ��ũ ���� ���
// =========================

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iostream>

#ifdef _WIN32
    // ������ ���� API ���
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
    // ���н� �迭 ���� ����
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using SOCKET = int;
constexpr int INVALID_SOCKET = -1;
#endif



// ==============================
// ��Ŷ ���
// ==============================

#include "PacketDef.h"



// ==============================
// ���� ���� ���
// ==============================
constexpr uint16_t SERVER_PORT = 9000;
constexpr size_t MAX_CLIENTS = 4;
constexpr size_t MAX_BUFFER_SIZE = 1024;
constexpr size_t MAX_NICKNAME_LENGTH = 16;


// ==============================
// ä�� ���� ���� ���
// ==============================
constexpr const char* CHAT_SERVER_IP = "127.0.0.1";
constexpr uint16_t CHAT_SERVER_PORT = 8001;



// ==============================
// ���� ���� ����ü
// - �� ���� Ŭ���̾�Ʈ�� ���� ����
// ==============================
struct UserSession {
    std::string ip;         // Ŭ���̾�Ʈ IP �ּ�
    uint16_t port;          // Ŭ���̾�Ʈ ��Ʈ ��ȣ
    SOCKET socket;          // ���� ��ȣ
    std::string nickname;   // Ŭ���̾�Ʈ �г���
    uint32_t uid = 0;       // ���� UID
    uint8_t modelId = 0;    // ������ �� ID
    bool isLoggedIn = false;// �α��� �Ϸ� ����

    // ������ ��� ���ڿ�
    std::string ToString() const {
        std::ostringstream oss;
        oss << "UID=" << uid
            << ", Nick=" << nickname
            << ", Model=" << static_cast<int>(modelId)
            << ", IP=" << ip
            << ", Port=" << port;
        return oss.str();
    }
};



// ==============================
// ä�� ���� ���� ����ü
// - ä�� ���� ���� Ŭ���̾�Ʈ�� ���� ����
// ==============================
struct ChatUserInfo
{
    uint32_t uid;
    std::string nickname;
    SOCKET socket;

    ChatUserInfo() = default;

    ChatUserInfo(uint32_t uid_, const std::string& nickname_, SOCKET socket_)
        : uid(uid_), nickname(nickname_), socket(socket_) {
    }
};


// ==============================
// ����/�г��� ���� �ڷᱸ�� Ÿ��
// ==============================
using UserMap = std::unordered_map<uint32_t, UserSession>;      // UID ��� ��ȸ
using NicknameMap = std::unordered_map<std::string, uint32_t>;  // �г��� �ߺ� �˻��
