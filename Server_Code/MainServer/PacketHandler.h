// PacketHandler.h
#pragma once
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

    // ... 필요 시 추가
};
