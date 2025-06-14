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
enum class EPacketType : uint16_t {
    None = 0,

    // �α��� ����: 1000����
    LOGIN_NICKNAME_REQ = 1001,
    LOGIN_NICKNAME_ACK = 1002,
    LOGIN_MODEL_SELECT_REQ = 1003,
    LOGIN_MODEL_SELECT_ACK = 1004,
    LOGIN_FINALIZE_REQ = 1005,
    LOGIN_SUCCESS_ACK = 1006,
    LOGOUT_REQ = 1007,

    // ä�� ����: 2000����
    CHAT_SEND = 2001,
    CHAT_RECV = 2002,

    // �ý��� �޽���
    ERROR_MSG = 9001
};



// ==============================
// ���� ���� ���
// ==============================
constexpr uint16_t SERVER_PORT = 9000;
constexpr size_t MAX_CLIENTS = 4;
constexpr size_t MAX_BUFFER_SIZE = 1024;
constexpr size_t MAX_NICKNAME_LENGTH = 16;
constexpr uint8_t MAX_MODEL_ID = 3;



// ==============================
// ���� ���� ����ü
// - �� ���� Ŭ���̾�Ʈ�� ���� ����
// ==============================
struct UserSession {
    std::string ip;         // Ŭ���̾�Ʈ IP �ּ�
    uint16_t port;          // Ŭ���̾�Ʈ ��Ʈ ��ȣ
    SOCKET socket;          // ���� ��ȣ
    std::string nickname;   // Ŭ���̾�Ʈ �г���
    uint32_t uid;           // ���� UID
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
// ����/�г��� ���� �ڷᱸ�� Ÿ��
// ==============================
using UserMap = std::unordered_map<uint32_t, UserSession>;      // UID ��� ��ȸ
using NicknameMap = std::unordered_map<std::string, uint32_t>;  // �г��� �ߺ� �˻��
