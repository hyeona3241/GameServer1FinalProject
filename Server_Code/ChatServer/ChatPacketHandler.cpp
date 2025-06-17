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
        std::cout << "[ChatPacketHandler] 채널 입장 요청 처리 중..." << std::endl;
        HandleChatEnterReq(socket, reader);
        break;
    case EPacketType::CHAT_SEND_REQ:
        std::cout << "[ChatPacketHandler] 채팅 메시지 수신 처리 중..." << std::endl;
        HandleChatSendReq(socket, reader);
        break;
    case EPacketType::CHAT_CHANNEL_LEAVE_REQ:
        std::cout << "[ChatPacketHandler] 채널 퇴장 요청 처리 중..." << std::endl;
        HandleChatLeaveReq(socket, reader);
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

void ChatPacketHandler::HandleChatSendReq(SOCKET, BinaryReader& reader)
{
    try {
        // 1. CHAT_SEND_REQ 역직렬화
        ChatSendReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        uint32_t uid = req.GetUID();
        uint64_t timestamp = req.GetTimestamp();
        const std::string& message = req.GetMessage();

        // 2. 메시지 로그 출력
        std::cout << "[ChatSendReq] UID: " << uid << " | Message: " << message << std::endl;

        // 3. CHAT_BROADCAST 패킷 생성
        ChatBroadcastPacket broadcast(uid, timestamp, message);
        std::vector<char> data = broadcast.Serialize();

        // 4. 메인 서버로 전송
        if (!ChatServer::GetInstance().SendPacket(data)) {
            std::cerr << "[ChatSendReq] 메인 서버로 메시지 전송 실패!" << std::endl;
        }
        else {
            std::cout << "[ChatSendReq] 메인 서버로 CHAT_BROADCAST 전송 완료." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ChatSendReq] 예외 발생: " << e.what() << std::endl;
    }
}

void ChatPacketHandler::HandleChatLeaveReq(SOCKET, BinaryReader& reader)
{
    try {
        // 1. UID 추출
        uint32_t uid = reader.ReadUInt32();

        // 2. 로그 출력
        std::cout << "[ChatLeaveReq] UID " << uid << " 유저가 퇴장하였습니다." << std::endl;

        // 3. LOGOUT_REQ 패킷 생성
        LogoutReqPacket logoutPacket(uid);
        std::vector<char> data = logoutPacket.Serialize();

        // 4. 메인 서버로 전송
        if (!ChatServer::GetInstance().SendPacket(data)) {
            std::cerr << "[ChatLeaveReq] 메인 서버로 LOGOUT_REQ 전송 실패 - UID: " << uid << std::endl;
        }
        else {
            std::cout << "[ChatLeaveReq] 메인 서버로 LOGOUT_REQ 전송 완료 - UID: " << uid << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ChatLeaveReq] 예외 발생: " << e.what() << std::endl;
    }
}



