#pragma once

#include "ServerCommon.h"
#include "BinaryReader.h"

class ChatPacketHandler
{
public:
	static void HandlePacket(SOCKET socket, const PacketHeader& header, BinaryReader& reader);

private:
	static void HandleChatEnterReq(SOCKET, BinaryReader&);
};

