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
    default:
    {
        ErrorPacket error(EErrorCode::INVALID_PACKET, "�������� �ʴ� ��Ŷ�Դϴ�.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
    break;
    }
}


void PacketHandler::HandleLoginNicknameReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        LoginNicknameReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        const std::string& nickname = req.GetNickname();

        LoginNicknameAckPacket ack;

        if (!SessionManager::GetInstance().IsNicknameDuplicate(nickname)) {
            // �г����� �̹� �����ϴ� ��� �� �ߺ� �÷��� true
            ack.SetIsDuplicate(true);
        }
        else {
            // ��� ������ �г��� �� �ߺ� �÷��� false, UID�� ���� �ο����� ����
            ack.SetIsDuplicate(false);
            ack.SetUID(0);  // ��Ģ�� UID�� ���߿� �ο�
        }

        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLoginNicknameReq] Exception: " << e.what() << std::endl;

        // Ŭ�� ���� ��Ŷ�� �ƴ� �Ϲ� ���� ��Ŷ�� ��ٸ��Ƿ�, ���� ��Ŷ�� ������ ����.
        // �α׸� ����� ���� or ����
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
                isValid = false; // ���� ������ ������ ��ȿ�� false�� ����
            }
        }

        LoginModelSelectAckPacket ack(isValid, modelId);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleModelSelect] Exception: " << e.what() << std::endl;
        // Ŭ�� ACK ��Ŷ�� ��ٸ��Ƿ�, ���� ��Ŷ�� ����
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

        // 2. UID �߱� �� ���
        uint32_t uid = SessionManager::GetInstance().RegisterNewUser(*session);

        // 3. ���� ��Ŷ ����
        LoginSuccessAckPacket ack(uid, session->nickname);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());

        std::cout << "[HandleLoginFinalize] �α��� �Ϸ�: UID = " << uid
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
        LogoutReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        uint32_t uid = req.GetUID();

        // ���� ���� �õ�
        bool success = SessionManager::GetInstance().RemoveUser(uid);

        if (success) {
            std::cout << "[HandleLogoutReq] ���� �α׾ƿ� ���� - UID: " << uid << std::endl;

            // �ʿ� ��: �ٸ� �������� ��ε�ĳ��Ʈ �˸� ó���� ����
        }
        else {
            std::cerr << "[HandleLogoutReq] ���� ���� ���� ���� - UID: " << uid << std::endl;

            // Ŭ�� ���� ��Ŷ ������ �͵� ��� ����
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

