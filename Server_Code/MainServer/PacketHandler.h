// PacketHandler.h
#pragma once
#include "BinaryReader.h"
#include "ServerCommon.h"

class PacketHandler
{
public:
    static void HandlePacket(SOCKET clientSocket, const PacketHeader& header, BinaryReader& reader);

private:
    static void HandleLoginNicknameReq(SOCKET, BinaryReader&);
    static void HandleModelSelect(SOCKET, BinaryReader&);
    static void HandleLoginFinalize(SOCKET, BinaryReader&);
    static void HandleLogoutReq(SOCKET, BinaryReader&);

    // ... 필요 시 추가
};
