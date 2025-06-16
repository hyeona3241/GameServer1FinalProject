#include "ChatPacketHandler.h"
#include "ChatServer.h"
#include "ErrorPacket.h"
#include "ChatChannelEnterReqPacket.h"
#include "ChatBroadcastPacket.h"
#include "ChatChannelEnterAckPacket.h"

void ChatPacketHandler::HandlePacket(SOCKET socket, const PacketHeader& header, BinaryReader& reader)
{
    switch (header.type)
    {
    case EPacketType::CHAT_CHANNEL_ENTER_REQ:
        std::cout << "[ChatPacketHandler] 채널 입장 요청 처리 중..." << std::endl;
        HandleChatEnterReq(socket, reader);
        break;
    case EPacketType::CHAT_SEND_REQ:
        std::cout << "[ChatPacketHandler] 채팅 메시지 수신 처리 중..." << std::endl;
        break;



    default:
        ErrorPacket error(EErrorCode::INVALID_PACKET, "지원하지 않는 패킷입니다.");
        ChatServer::GetInstance().SendPacket(error.Serialize());
        std::cerr << "[ChatPacketHandler] 알 수 없는 패킷 ID: " << static_cast<uint16_t>(header.type) << std::endl;
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
        std::cerr << "[ChatEnterReq] CHAT_CHANNEL_ENTER_ACK 패킷 전송 실패" << std::endl;
    }
    else {
        std::cout << "[ChatEnterReq] CHAT_CHANNEL_ENTER_ACK 전송 완료 - UID: " << uid << ", Timestamp: " << timestamp << std::endl;
    }
}

