#pragma once

// PacketHandler 클래스
// ----------------------
// 이 클래스는 서버에서 수신한 네트워크 패킷을 처리하는 정적 유틸리티 클래스입니다.
// 각 패킷의 타입(EPacketType)에 따라 적절한 핸들러 함수를 호출하는 역할을 합니다.
// 일반적으로 클라이언트가 전송한 요청 패킷을 분기하여 처리하며,
// 다음 기능을 제공합니다:
//
// - 패킷 타입 기반 라우팅 (HandlePacket 함수)
// - 로그인, 채팅, 로그아웃 등 각 기능별 처리 함수 구현
// - BinaryReader를 통해 받은 데이터를 적절한 Packet 객체로 변환하여 처리

#include "BinaryReader.h"
#include "ChatManager.h"
#include "ServerCommon.h"

class PacketHandler
{
public:
    static void HandlePacket(SOCKET clientSocket, const PacketHeader& header, BinaryReader& reader);
    static void HandleChatPacket(SOCKET chatSocket, const PacketHeader& header, BinaryReader& reader);

private:
    static void HandleLoginNicknameReq(SOCKET, BinaryReader&);
    static void HandleModelSelect(SOCKET, BinaryReader&);
    static void HandleLoginFinalize(SOCKET, BinaryReader&);
    static void HandleLogoutReq(SOCKET, BinaryReader&);

    static void HandleChatEnterReq(SOCKET, BinaryReader&);
    static void HandleChatBrpadcast(SOCKET, BinaryReader&);
    static void HandleChatEnterAck(SOCKET, BinaryReader&);
    static void HandleChatSendReq(SOCKET, BinaryReader&);
    static void HandleChatLeaveReq(SOCKET, BinaryReader&);

};
