// PacketHandler.cpp
#include "PacketHandler.h"
#include "NetworkManager.h"
#include "SessionManager.h"
#include "LoginNicknameReqPacket.h"
#include "LoginNicknameAckPacket.h"
#include "LoginSuccessAckPacket.h"
#include "LogoutReqPacket.h"
#include "LoginModelSelectReqPacket.h"
#include "LoginModelSelectAckPacket.h"
#include "LoginFinalizeReqPacket.h"
#include "ErrorPacket.h"
#include "ChatChannelEnterReqPacket.h"
#include "ChatChannelEnterAckPacket.h"
#include "ChatBroadcastPacket.h"
#include "ChatSendReqPacket.h"
#include "ChatChannelLeaveReqPacket.h"

// 클라이언트로 부터 받는 패킷
void PacketHandler::HandlePacket(SOCKET clientSocket, const PacketHeader& header, BinaryReader& reader)
{
    switch (header.type)
    {
    case EPacketType::LOGIN_NICKNAME_REQ:
        HandleLoginNicknameReq(clientSocket, reader);
        break;
    case EPacketType::LOGIN_MODEL_SELECT_REQ:
        HandleModelSelect(clientSocket, reader);
        break;
    case EPacketType::LOGIN_FINALIZE_REQ:
        HandleLoginFinalize(clientSocket, reader);
        break;
    case EPacketType::LOGOUT_REQ:
        HandleLogoutReq(clientSocket, reader);
        break;
    case EPacketType::CHAT_CHANNEL_ENTER_REQ :
        HandleChatEnterReq(clientSocket, reader);
        break;
    case EPacketType::CHAT_SEND_REQ:
        HandleChatSendReq(clientSocket, reader);
        break;
    case EPacketType::CHAT_CHANNEL_LEAVE_REQ:
        HandleChatLeaveReq(clientSocket, reader);
        break;

    default:
    {
        ErrorPacket error(EErrorCode::INVALID_PACKET, "지원하지 않는 패킷입니다.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
    break;
    }
}


// 채팅 서버로부터 받는 패킷
void PacketHandler::HandleChatPacket(SOCKET chatSocket, const PacketHeader& header, BinaryReader& reader)
{
    switch (header.type)
    {
    case EPacketType::CHAT_BROADCAST:
        HandleChatBrpadcast(chatSocket, reader);
        break;

    case EPacketType::CHAT_CHANNEL_ENTER_ACK:
        HandleChatEnterAck(chatSocket, reader);
        break;

    default:
        ErrorPacket error(EErrorCode::INVALID_PACKET, "지원하지 않는 패킷입니다.");
        NetworkManager::GetInstance().SendPacket(chatSocket, error.Serialize());
        std::cerr << "[Chat] 알 수 없는 채팅 패킷 타입입니다: " << static_cast<uint16_t>(header.type) << std::endl;
        break;
    }
}



// [HandleLoginNicknameReq]
// 클라이언트가 보낸 닉네임을 검증하고 중복 여부를 확인합니다.
// 중복이 아니면 UID를 발급하고 세션에 등록한 후, ACK 패킷을 전송합니다.
void PacketHandler::HandleLoginNicknameReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        std::string nickname = reader.ReadString();

        if (nickname.empty() || nickname.length() > 16) {
            throw std::invalid_argument("Invalid nickname");
        }

        LoginNicknameAckPacket ack;

        if (SessionManager::GetInstance().IsNicknameDuplicate(nickname)) {
            ack.SetIsDuplicate(true);
        }
        else {
            ack.SetIsDuplicate(false);

            // 세션 생성 및 UID 발급
            UserSession session;
            session.nickname = nickname;
            session.socket = clientSocket;
            session.modelId = 0; // 기본값

            uint32_t uid = SessionManager::GetInstance().RegisterNewUser(session);
            SessionManager::GetInstance().AddUser(clientSocket, session);

            ack.SetUID(uid);
        }

        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLoginNicknameReq] Exception: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::INVALID_NICKNAME, "Nickname must be 1~16 chars.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
}


// [HandleModelSelect]
// 클라이언트가 선택한 캐릭터 모델 ID를 수신하여 해당 세션에 반영합니다.
// 유효한 모델 ID인 경우 세션에 저장 후 성공 응답을 전송하며,
// 유효하지 않거나 세션이 없을 경우 실패 응답을 전송합니다.
void PacketHandler::HandleModelSelect(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        LoginModelSelectReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());
        uint8_t modelId = req.GetModelId();

        bool isValid = (modelId <= MAX_MODEL_ID);

        if (isValid) {
            UserSession* session = SessionManager::GetInstance().GetUserBySocket(clientSocket);
            if (session) {
                session->modelId = modelId;
            }
            else {
                std::cerr << "[HandleModelSelect] 유저 세션을 찾을 수 없습니다." << std::endl;
                isValid = false;
            }
        }

        LoginModelSelectAckPacket ack(isValid, modelId);
        std::vector<char> buffer = ack.Serialize();

        // 전송할 바이트 로그 출력 (디버그용)
        std::cout << "[ModelSelectAck] Sending Packet (size = " << buffer.size() << "): ";
        for (unsigned char c : buffer)
            printf("%02X-", c);
        std::cout << std::endl;

        NetworkManager::GetInstance().SendPacket(clientSocket, buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleModelSelect] Exception: " << e.what() << std::endl;
        //에러 패킷 추가 가능
    }
}


// [HandleLoginFinalizeReq]
// 클라이언트가 최종적으로 선택한 모델 ID를 수신하여 세션에 저장합니다.
// 유효성 검사 후 완료 패킷 또는 오류 패킷을 응답합니다.
void PacketHandler::HandleLoginFinalize(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        LoginFinalizeReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        UserSession* session = SessionManager::GetInstance().GetUserBySocket(clientSocket);
        if (session == nullptr) {
            std::cerr << "[HandleLoginFinalize] 세션을 찾을 수 없습니다." << std::endl;
            return;
        }

        if (session->uid == 0) {
            std::cerr << "[HandleLoginFinalize] UID가 할당되지 않은 세션입니다." << std::endl;
            return;
        }

        static bool chatConnected = false;
        if (!chatConnected) {
            chatConnected = ChatManager::GetInstance().ConnectToChatServer(CHAT_SERVER_IP, CHAT_SERVER_PORT);
            if (!chatConnected) {
                std::cerr << "[HandleLoginFinalize] 채팅 서버 연결 실패." << std::endl;
                
            }
        }

        LoginSuccessAckPacket ack(session->uid, session->nickname);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());

        std::cout << "[HandleLoginFinalize] 로그인 완료: UID = " << session->uid
            << ", 닉네임 = " << session->nickname << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLoginFinalize] Exception: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::LOGIN_FAILED, "로그인 최종 단계에서 오류가 발생했습니다.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
}


// [HandleLogoutReq]
// UID 기반으로 세션과 채팅 유저 정보를 제거합니다.
// 퇴장 브로드캐스트 메시지를 모든 접속자에게 전송합니다.
void PacketHandler::HandleLogoutReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        uint32_t uid = reader.ReadUInt32();

        std::string nickname = ChatManager::GetInstance().GetNicknameByUID(uid);
        if (nickname.empty()) {
            std::cerr << "[HandleLogoutReq] UID로 닉네임을 찾을 수 없습니다. UID: " << uid << std::endl;
            return;
        }

        std::string message = "<" + nickname + "> has left the chat.";
        uint64_t timestamp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());

        ChatBroadcastPacket broadcast(uid, timestamp, message);
        std::vector<char> data = broadcast.Serialize();

        const auto& clients = ChatManager::GetInstance().GetAllClientSockets();
        for (SOCKET sock : clients) {
            if (send(sock, data.data(), static_cast<int>(data.size()), 0) == SOCKET_ERROR) {
                std::cerr << "[HandleLogoutReq] 클라이언트 소켓 전송 실패: " << sock << std::endl;
            }
        }

        std::cout << "[HandleLogoutReq] 퇴장 메시지 브로드캐스트 완료: " << message << std::endl;

        ChatManager::GetInstance().RemoveChatUser(uid);
        std::cout << "[HandleLogoutReq] UID " << uid << " 채팅 사용자 정보 제거 완료" << std::endl;

        bool success = SessionManager::GetInstance().RemoveUser(uid);
        if (success) {
            std::cout << "[HandleLogoutReq] 유저 로그아웃 성공 - UID: " << uid << std::endl;
        }
        else {
            std::cerr << "[HandleLogoutReq] 유저 정보 삭제 실패 - UID: " << uid << std::endl;

            ErrorPacket error(EErrorCode::INVALID_UID, "로그아웃 처리 중 UID 정보가 유효하지 않습니다.");
            NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLogoutReq] 예외 발생: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "로그아웃 패킷 처리 중 예외가 발생했습니다.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
}



// [HandleChatEnterReq]
// 클라이언트의 채팅 채널 입장 요청을 채팅 서버로 전달합니다.
void PacketHandler::HandleChatEnterReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        ChatChannelEnterReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());
        uint32_t uid = req.GetUID();

        UserSession* session = SessionManager::GetInstance().GetUserByUID(uid);
        if (session == nullptr) {
            std::cerr << "[HandleChatEnterReq] UID가 유효하지 않음: " << uid << std::endl;

            ErrorPacket error(EErrorCode::INVALID_UID, "유효하지 않은 UID입니다.");
            ChatManager::GetInstance().SendPacket(error);
            return;
        }

        ChatManager::GetInstance().AddChatUser(uid, session->nickname, clientSocket);

        ChatChannelEnterReqPacket forwardPacket(uid, req.GetTimestamp());
        ChatManager::GetInstance().SendPacket(forwardPacket);

        std::cout << "[HandleChatEnterReq] UID " << uid << " (" << session->nickname << ") 채팅 서버 입장 요청 전송됨" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatEnterReq] 예외 발생: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "채팅 입장 처리 중 오류가 발생했습니다.");
        ChatManager::GetInstance().SendPacket(error);
    }
}


// [HandleChatBroadcast]
// 채팅 서버에서 받은 CHAT_BROADCAST 메시지를 클라이언트들에게 전송합니다.
void PacketHandler::HandleChatBrpadcast(SOCKET, BinaryReader& reader)
{
    try {
        const char* rawBuffer = reader.GetBuffer();
        size_t bufferSize = reader.GetBufferSize();

        for (const auto& pair : ChatManager::GetInstance().GetAllChatUsers())
        {
            SOCKET clientSocket = pair.second.socket;

            int totalSent = 0;
            while (totalSent < bufferSize) 
            {
                int sent = send(clientSocket, rawBuffer + totalSent, static_cast<int>(bufferSize - totalSent), 0);
                if (sent == SOCKET_ERROR) 
                {
                    std::cerr << "[HandleChatBroadcast] 전송 실패: UID = " << pair.first << std::endl;
                    break;
                }
                totalSent += sent;
            }
        }

        std::cout << "[HandleChatBroadcast] 모든 채팅 클라이언트에게 메시지 전송 완료\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatBroadcast] 예외 발생: " << e.what() << std::endl;
    }

}


// [HandleChatEnterAck]
// 채팅 서버에서 받은 입장 ACK 패킷을 처리하고,
// "[닉네임]님이 입장하였습니다." 메시지를 모든 클라이언트에게 브로드캐스트합니다.
void PacketHandler::HandleChatEnterAck(SOCKET, BinaryReader& reader)
{
    ChatChannelEnterAckPacket ack;
    ack.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

    uint32_t uid = ack.GetUID();
    uint64_t timestamp = ack.GetTimestamp();

    std::string nickname = ChatManager::GetInstance().GetNicknameByUID(uid);
    if (nickname.empty()) {
        std::cerr << "[HandleChatEnterAck] UID에 해당하는 닉네임을 찾을 수 없습니다. UID: " << uid << std::endl;
        return;
    }

    std::string message = "<" + nickname + "> has entered the chat.";

    ChatBroadcastPacket packet(uid, timestamp, message);
    std::vector<char> data = packet.Serialize();

    const auto& allSockets = ChatManager::GetInstance().GetAllClientSockets();
    for (SOCKET clientSocket : allSockets) {
        if (clientSocket == INVALID_SOCKET) continue;

        int sent = send(clientSocket, data.data(), static_cast<int>(data.size()), 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "[HandleChatEnterAck] 클라이언트 전송 실패: " << clientSocket << std::endl;
        }
    }

    std::cout << "[HandleChatEnterAck] " << nickname << " 입장 브로드캐스트 완료" << std::endl;
}


// [HandleChatSendReq]
// 클라이언트가 입력한 채팅 메시지를 채팅 서버로 전달합니다
void PacketHandler::HandleChatSendReq(SOCKET, BinaryReader& reader)
{
    try {
        ChatSendReqPacket packet;
        packet.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        std::cout << "[HandleChatSendReq] 메시지 수신 - UID: " << packet.GetUID()
            << ", 메시지: " << packet.GetMessage() << std::endl;

        std::vector<char> data = packet.Serialize(); 
        ChatManager::GetInstance().SendPacket(packet);

        std::cout << "[HandleChatSendReq] 채팅 서버로 전달 완료\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatSendReq] 예외 발생: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "채팅 메시지 처리 중 오류 발생");
        ChatManager::GetInstance().SendPacket(error);
    }
}


// [HandleChatLeaveReq]
// 클라이언트의 채널 퇴장 요청을 채팅 서버로 전달합니다.
void PacketHandler::HandleChatLeaveReq(SOCKET, BinaryReader& reader)
{
    try {
        uint32_t uid = reader.ReadUInt32();
        uint64_t timestamp = reader.ReadUInt64();

        ChatChannelLeaveReqPacket packet(uid, timestamp);
        auto data = packet.Serialize();

        ChatManager::GetInstance().SendPacket(packet);

        std::cout << "[HandleChatLeaveReq] 채팅 서버로 CHAT_CHANNEL_LEAVE_REQ 전송 - UID: " << uid << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatLeaveReq] 예외 발생: " << e.what() << std::endl;
    }
}
