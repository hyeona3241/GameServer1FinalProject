#pragma once

// ChatPacketHandler Ŭ����
// ------------------------
// ä�� �������� ���ŵ� ��Ŷ�� ó���ϴ� ���� ��ƿ��Ƽ Ŭ�����Դϴ�.
// PacketHeader�� Ÿ�Կ� ���� ������ ó�� �Լ��� �б��ϸ�, �� �ڵ鷯�� ������ �����մϴ�.

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

