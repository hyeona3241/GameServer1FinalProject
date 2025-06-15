#pragma once

// =========================
// 서버 공통 헤더 정의 파일
// - 유저 정보 구조체
// - 패킷 타입 정의
// - 서버 네트워크 설정 상수
// =========================

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iostream>

#ifdef _WIN32
    // 윈도우 소켓 API 사용
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
    // 유닉스 계열 소켓 설정
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using SOCKET = int;
constexpr int INVALID_SOCKET = -1;
#endif



// ==============================
// 패킷 사용
// ==============================

#include "PacketDef.h"



// ==============================
// 서버 설정 상수
// ==============================
constexpr uint16_t SERVER_PORT = 9000;
constexpr size_t MAX_CLIENTS = 4;
constexpr size_t MAX_BUFFER_SIZE = 1024;
constexpr size_t MAX_NICKNAME_LENGTH = 16;



// ==============================
// 유저 세션 구조체
// - 각 접속 클라이언트의 상태 저장
// ==============================
struct UserSession {
    std::string ip;         // 클라이언트 IP 주소
    uint16_t port;          // 클라이언트 포트 번호
    SOCKET socket;          // 소켓 번호
    std::string nickname;   // 클라이언트 닉네임
    uint32_t uid;           // 고유 UID
    uint8_t modelId = 0;    // 선택한 모델 ID
    bool isLoggedIn = false;// 로그인 완료 여부

    // 디버깅용 출력 문자열
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
// 세션/닉네임 관리 자료구조 타입
// ==============================
using UserMap = std::unordered_map<uint32_t, UserSession>;      // UID 기반 조회
using NicknameMap = std::unordered_map<std::string, uint32_t>;  // 닉네임 중복 검사용
