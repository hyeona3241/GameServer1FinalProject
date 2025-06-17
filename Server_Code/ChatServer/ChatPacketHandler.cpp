#include "ChatPacketHandler.h"
#include "ChatServer.h"
#include "ErrorPacket.h"
#include "ChatChannelEnterReqPacket.h"
#include "ChatBroadcastPacket.h"
#include "ChatChannelEnterAckPacket.h"
#include "ChatSendReqPacket.h"
#include "LogoutReqPacket.h"

void ChatPacketHandler::HandlePacket(SOCKET socket, const PacketHeader& header, BinaryReader& reader)
{
    switch (header.type)
    {
    case EPacketType::CHAT_CHANNEL_ENTER_REQ:
        std::cout << "[ChatPacketHandler] ä�� ���� ��û ó�� ��..." << std::endl;
        HandleChatEnterReq(socket, reader);
        break;
    case EPacketType::CHAT_SEND_REQ:
        std::cout << "[ChatPacketHandler] ä�� �޽��� ���� ó�� ��..." << std::endl;
        HandleChatSendReq(socket, reader);
        break;
    case EPacketType::CHAT_CHANNEL_LEAVE_REQ:
        std::cout << "[ChatPacketHandler] ä�� ���� ��û ó�� ��..." << std::endl;
        HandleChatLeaveReq(socket, reader);
        break;


    default:
        ErrorPacket error(EErrorCode::INVALID_PACKET, "�������� �ʴ� ��Ŷ�Դϴ�.");
        ChatServer::GetInstance().SendPacket(error.Serialize());
        std::cerr << "[ChatPacketHandler] �� �� ���� ��Ŷ ID: " << static_cast<uint16_t>(header.type) << std::endl;
        break;
    }
}

void ChatPacketHandler::HandleChatEnterReq(SOCKET socket, BinaryReader& reader)
{
    ChatChannelEnterReqPacket req;
    req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());


    uint32_t uid = req.GetUID();
    uint64_t timestamp = req.GetTimestamp();


    ChatChannelEnterAckPacket ackPacket(uid, timestamp);
    std::vector<char> ackData = ackPacket.Serialize();

    if (!ChatServer::GetInstance().SendPacket(ackData)) {
        std::cerr << "[ChatEnterReq] CHAT_CHANNEL_ENTER_ACK ��Ŷ ���� ����" << std::endl;
    }
    else {
        std::cout << "[ChatEnterReq] CHAT_CHANNEL_ENTER_ACK ���� �Ϸ� - UID: " << uid << ", Timestamp: " << timestamp << std::endl;
    }
}

void ChatPacketHandler::HandleChatSendReq(SOCKET, BinaryReader& reader)
{
    try {
        // 1. CHAT_SEND_REQ ������ȭ
        ChatSendReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        uint32_t uid = req.GetUID();
        uint64_t timestamp = req.GetTimestamp();
        const std::string& message = req.GetMessage();

        // 2. �޽��� �α� ���
        std::cout << "[ChatSendReq] UID: " << uid << " | Message: " << message << std::endl;

        // 3. CHAT_BROADCAST ��Ŷ ����
        ChatBroadcastPacket broadcast(uid, timestamp, message);
        std::vector<char> data = broadcast.Serialize();

        // 4. ���� ������ ����
        if (!ChatServer::GetInstance().SendPacket(data)) {
            std::cerr << "[ChatSendReq] ���� ������ �޽��� ���� ����!" << std::endl;
        }
        else {
            std::cout << "[ChatSendReq] ���� ������ CHAT_BROADCAST ���� �Ϸ�." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ChatSendReq] ���� �߻�: " << e.what() << std::endl;
    }
}

void ChatPacketHandler::HandleChatLeaveReq(SOCKET, BinaryReader& reader)
{
    try {
        // 1. UID ����
        uint32_t uid = reader.ReadUInt32();

        // 2. �α� ���
        std::cout << "[ChatLeaveReq] UID " << uid << " ������ �����Ͽ����ϴ�." << std::endl;

        // 3. LOGOUT_REQ ��Ŷ ����
        LogoutReqPacket logoutPacket(uid);
        std::vector<char> data = logoutPacket.Serialize();

        // 4. ���� ������ ����
        if (!ChatServer::GetInstance().SendPacket(data)) {
            std::cerr << "[ChatLeaveReq] ���� ������ LOGOUT_REQ ���� ���� - UID: " << uid << std::endl;
        }
        else {
            std::cout << "[ChatLeaveReq] ���� ������ LOGOUT_REQ ���� �Ϸ� - UID: " << uid << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ChatLeaveReq] ���� �߻�: " << e.what() << std::endl;
    }
}



