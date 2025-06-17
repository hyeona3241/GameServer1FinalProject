#pragma once

// ChatPacketHandler 클래스
// ------------------------
// 채팅 서버에서 수신된 패킷을 처리하는 정적 유틸리티 클래스입니다.
// PacketHeader의 타입에 따라 적절한 처리 함수로 분기하며, 각 핸들러는 역할을 수행합니다.

#include "ServerCommon.h"
#include "BinaryReader.h"

class ChatPacketHandler
{
public:
	static void HandlePacket(SOCKET socket, const PacketHeader& header, BinaryReader& reader);

private:
	static void HandleChatEnterReq(SOCKET, BinaryReader&);
	static void HandleChatSendReq(SOCKET, BinaryReader&);
	static void HandleChatLeaveReq(SOCKET, BinaryReader&);
};

