#pragma once

// PacketHandler Ŭ����
// ----------------------
// �� Ŭ������ �������� ������ ��Ʈ��ũ ��Ŷ�� ó���ϴ� ���� ��ƿ��Ƽ Ŭ�����Դϴ�.
// �� ��Ŷ�� Ÿ��(EPacketType)�� ���� ������ �ڵ鷯 �Լ��� ȣ���ϴ� ������ �մϴ�.
// �Ϲ������� Ŭ���̾�Ʈ�� ������ ��û ��Ŷ�� �б��Ͽ� ó���ϸ�,
// ���� ����� �����մϴ�:
//
// - ��Ŷ Ÿ�� ��� ����� (HandlePacket �Լ�)
// - �α���, ä��, �α׾ƿ� �� �� ��ɺ� ó�� �Լ� ����
// - BinaryReader�� ���� ���� �����͸� ������ Packet ��ü�� ��ȯ�Ͽ� ó��

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
