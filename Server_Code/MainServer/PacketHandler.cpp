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
        ErrorPacket error(EErrorCode::INVALID_PACKET, "�������� �ʴ� ��Ŷ�Դϴ�.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
    break;
    }
}

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
        ErrorPacket error(EErrorCode::INVALID_PACKET, "�������� �ʴ� ��Ŷ�Դϴ�.");
        NetworkManager::GetInstance().SendPacket(chatSocket, error.Serialize());
        std::cerr << "[Chat] �� �� ���� ä�� ��Ŷ Ÿ���Դϴ�: " << static_cast<uint16_t>(header.type) << std::endl;
        break;
    }
}


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

            // ���� ���� �� UID �߱�
            UserSession session;
            session.nickname = nickname;
            session.socket = clientSocket;
            session.modelId = 0; // �⺻��

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
                std::cerr << "[HandleModelSelect] ���� ������ ã�� �� �����ϴ�." << std::endl;
                isValid = false;
            }
        }

        LoginModelSelectAckPacket ack(isValid, modelId);
        std::vector<char> buffer = ack.Serialize();

        // ������ ����Ʈ �α� ���
        std::cout << "[ModelSelectAck] Sending Packet (size = " << buffer.size() << "): ";
        for (unsigned char c : buffer)
            printf("%02X-", c);
        std::cout << std::endl;

        NetworkManager::GetInstance().SendPacket(clientSocket, buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleModelSelect] Exception: " << e.what() << std::endl;
        //���� ��Ŷ �߰�
    }
}



void PacketHandler::HandleLoginFinalize(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        LoginFinalizeReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        // 1. ���� ��������
        UserSession* session = SessionManager::GetInstance().GetUserBySocket(clientSocket);
        if (session == nullptr) {
            std::cerr << "[HandleLoginFinalize] ������ ã�� �� �����ϴ�." << std::endl;
            return;
        }

        // 2. UID Ȯ�� �� ����
        if (session->uid == 0) {
            std::cerr << "[HandleLoginFinalize] UID�� �Ҵ���� ���� �����Դϴ�." << std::endl;
            return;
        }

        // ä�� ���� ���� �õ� (���� 1ȸ�� ����ǵ���)
        static bool chatConnected = false;
        if (!chatConnected) {
            chatConnected = ChatManager::GetInstance().ConnectToChatServer(CHAT_SERVER_IP, CHAT_SERVER_PORT);
            if (!chatConnected) {
                std::cerr << "[HandleLoginFinalize] ä�� ���� ���� ����." << std::endl;
                // �ʿ� �� Ŭ�󿡰� ���� ��Ŷ ����
            }
        }

        // 3. �α��� ���� ���� ����
        LoginSuccessAckPacket ack(session->uid, session->nickname);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());

        std::cout << "[HandleLoginFinalize] �α��� �Ϸ�: UID = " << session->uid
            << ", �г��� = " << session->nickname << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLoginFinalize] Exception: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::LOGIN_FAILED, "�α��� ���� �ܰ迡�� ������ �߻��߽��ϴ�.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
}

void PacketHandler::HandleLogoutReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        // BinaryReader�� �̹� ����� ���� ���¶�� ����
        uint32_t uid = reader.ReadUInt32();  // UID ���� ����

        // ���� ���� �õ�
        ChatManager::GetInstance().RemoveChatUser(uid);
        std::cout << "[HandleChatLeaveReq] UID " << uid << " ����� ���� ���� �Ϸ�" << std::endl;

        bool success = SessionManager::GetInstance().RemoveUser(uid);

        if (success) {
            std::cout << "[HandleLogoutReq] ���� �α׾ƿ� ���� - UID: " << uid << std::endl;

            std::string nickname = ChatManager::GetInstance().GetNicknameByUID(uid);
            if (nickname.empty()) {
                std::cerr << "[HandleLogoutReq] UID�� �г����� ã�� �� �����ϴ�. UID: " << uid << std::endl;
                return;
            }

            //  ��ε�ĳ��Ʈ �޽��� ����
            std::string message = "[" + nickname + "]���� �����Ͽ����ϴ�.";
            uint64_t timestamp = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();

            ChatBroadcastPacket broadcast(uid, timestamp, message);
            std::vector<char> data = broadcast.Serialize();

            //  ��� Ŭ���̾�Ʈ���� ����
            const auto& clients = ChatManager::GetInstance().GetAllClientSockets();
            for (SOCKET clientSocket : clients) {
                if (send(clientSocket, data.data(), static_cast<int>(data.size()), 0) == SOCKET_ERROR) {
                    std::cerr << "[HandleLogoutReq] Ŭ���̾�Ʈ ���� ���� ����: " << clientSocket << std::endl;
                }
            }

            std::cout << "[HandleLogoutReq] ���� �޽��� ��ε�ĳ��Ʈ �Ϸ�: " << message << std::endl;

        }
        else {
            std::cerr << "[HandleLogoutReq] ���� ���� ���� ���� - UID: " << uid << std::endl;

            ErrorPacket error(EErrorCode::INVALID_UID, "�α׾ƿ� ó�� �� UID ������ ��ȿ���� �ʽ��ϴ�.");
            NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLogoutReq] ���� �߻�: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "�α׾ƿ� ��Ŷ ó�� �� ���ܰ� �߻��߽��ϴ�.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
}

void PacketHandler::HandleChatEnterReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        // 1. ��Ŷ ������ȭ
        ChatChannelEnterReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());
        uint32_t uid = req.GetUID();

        // 2. UID ��� ���� ���� Ȯ��
        UserSession* session = SessionManager::GetInstance().GetUserByUID(uid);
        if (session == nullptr) {
            std::cerr << "[HandleChatEnterReq] UID�� ��ȿ���� ����: " << uid << std::endl;

            ErrorPacket error(EErrorCode::INVALID_UID, "��ȿ���� ���� UID�Դϴ�.");
            ChatManager::GetInstance().SendPacket(error);
            return;
        }

        // 3. ChatManager�� ���� ���
        ChatManager::GetInstance().AddChatUser(uid, session->nickname, clientSocket);

        // 4. ä�� ������ ���� ��Ŷ ����
        ChatChannelEnterReqPacket forwardPacket(uid, req.GetTimestamp());
        ChatManager::GetInstance().SendPacket(forwardPacket);

        std::cout << "[HandleChatEnterReq] UID " << uid << " (" << session->nickname << ") ä�� ���� ���� ��û ���۵�" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatEnterReq] ���� �߻�: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "ä�� ���� ó�� �� ������ �߻��߽��ϴ�.");
        ChatManager::GetInstance().SendPacket(error);
    }
}



void PacketHandler::HandleChatBrpadcast(SOCKET, BinaryReader& reader)
{
    try {
        // 1. ���ŵ� ������ ��ü�� �״�� ���۷� ����
        const char* rawBuffer = reader.GetBuffer();
        size_t bufferSize = reader.GetBufferSize();

        // 2. UID ������� ��ü ������ ��� ��ȸ
        for (const auto& pair : ChatManager::GetInstance().GetAllChatUsers())
        {
            SOCKET clientSocket = pair.second.socket;

            int totalSent = 0;
            while (totalSent < bufferSize) 
            {
                int sent = send(clientSocket, rawBuffer + totalSent, static_cast<int>(bufferSize - totalSent), 0);
                if (sent == SOCKET_ERROR) 
                {
                    std::cerr << "[HandleChatBroadcast] ���� ����: UID = " << pair.first << std::endl;
                    break;
                }
                totalSent += sent;
            }
        }

        std::cout << "[HandleChatBroadcast] ��� ä�� Ŭ���̾�Ʈ���� �޽��� ���� �Ϸ�\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatBroadcast] ���� �߻�: " << e.what() << std::endl;
    }

}


void PacketHandler::HandleChatEnterAck(SOCKET, BinaryReader& reader)
{
    // 1. ��Ŷ ������ȭ
    ChatChannelEnterAckPacket ack;
    ack.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

    uint32_t uid = ack.GetUID();
    uint64_t timestamp = ack.GetTimestamp();

    // 2. �г��� ��ȸ
    std::string nickname = ChatManager::GetInstance().GetNicknameByUID(uid);
    if (nickname.empty()) {
        std::cerr << "[HandleChatEnterAck] UID�� �ش��ϴ� �г����� ã�� �� �����ϴ�. UID: " << uid << std::endl;
        return;
    }

    // 3. �޽��� ����
    std::string message = "<" + nickname + "> has entered the chat.";

    // 4. ��Ŷ ���� �� ����ȭ
    ChatBroadcastPacket packet(uid, timestamp, message);
    std::vector<char> data = packet.Serialize();

    // 5. ��ü ���� ��ȸ�Ͽ� ��ε�ĳ��Ʈ
    const auto& allSockets = ChatManager::GetInstance().GetAllClientSockets();
    for (SOCKET clientSocket : allSockets) {
        if (clientSocket == INVALID_SOCKET) continue;

        int sent = send(clientSocket, data.data(), static_cast<int>(data.size()), 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "[HandleChatEnterAck] Ŭ���̾�Ʈ ���� ����: " << clientSocket << std::endl;
        }
    }

    std::cout << "[HandleChatEnterAck] " << nickname << " ���� ��ε�ĳ��Ʈ �Ϸ�" << std::endl;
}

void PacketHandler::HandleChatSendReq(SOCKET, BinaryReader& reader)
{
    try {
        ChatSendReqPacket packet;
        packet.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        std::cout << "[HandleChatSendReq] �޽��� ���� - UID: " << packet.GetUID()
            << ", �޽���: " << packet.GetMessage() << std::endl;

        std::vector<char> data = packet.Serialize(); // �ٽ� ���μ� ����ȭ
        ChatManager::GetInstance().SendPacket(packet); // �Ǵ� SendPacket(packet);

        std::cout << "[HandleChatSendReq] ä�� ������ ���� �Ϸ�\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatSendReq] ���� �߻�: " << e.what() << std::endl;

        ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "ä�� �޽��� ó�� �� ���� �߻�");
        ChatManager::GetInstance().SendPacket(error);
    }
}

void PacketHandler::HandleChatLeaveReq(SOCKET, BinaryReader& reader)
{
    try {
        // 1. ������ �Ľ�
        uint32_t uid = reader.ReadUInt32();
        uint64_t timestamp = reader.ReadUInt64();

        // 2. ��Ŷ ����
        ChatChannelLeaveReqPacket packet(uid, timestamp);
        auto data = packet.Serialize();

        // 3. ä�� ������ ����
        ChatManager::GetInstance().SendPacket(packet);  // �Ǵ� SendRawPacket(data);

        std::cout << "[HandleChatLeaveReq] ä�� ������ CHAT_CHANNEL_LEAVE_REQ ���� - UID: " << uid << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleChatLeaveReq] ���� �߻�: " << e.what() << std::endl;
    }
}


