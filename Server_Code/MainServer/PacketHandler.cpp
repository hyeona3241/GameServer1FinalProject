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
        ErrorPacket error(EErrorCode::INVALID_PACKET, "지원하지 않는 패킷입니다.");
        NetworkManager::GetInstance().SendPacket(clientSocket, error.Serialize());
    }
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

        // 전송할 바이트 로그 출력
        std::cout << "[ModelSelectAck] Sending Packet (size = " << buffer.size() << "): ";
        for (unsigned char c : buffer)
            printf("%02X-", c);
        std::cout << std::endl;

        NetworkManager::GetInstance().SendPacket(clientSocket, buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleModelSelect] Exception: " << e.what() << std::endl;
        //에러 패킷 추가
    }
}



void PacketHandler::HandleLoginFinalize(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        LoginFinalizeReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        // 1. 세션 가져오기
        UserSession* session = SessionManager::GetInstance().GetUserBySocket(clientSocket);
        if (session == nullptr) {
            std::cerr << "[HandleLoginFinalize] 세션을 찾을 수 없습니다." << std::endl;
            return;
        }

        // 2. UID 확인 후 응답
        if (session->uid == 0) {
            std::cerr << "[HandleLoginFinalize] UID가 할당되지 않은 세션입니다." << std::endl;
            return;
        }

        // 3. 로그인 성공 응답 전송
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




void PacketHandler::HandleLogoutReq(SOCKET clientSocket, BinaryReader& reader)
{
    try {
        // BinaryReader는 이미 헤더를 읽은 상태라고 가정
        uint32_t uid = reader.ReadUInt32();  // UID 직접 추출

        // 세션 제거 시도
        bool success = SessionManager::GetInstance().RemoveUser(uid);

        if (success) {
            std::cout << "[HandleLogoutReq] 유저 로그아웃 성공 - UID: " << uid << std::endl;

            // 필요 시: 다른 유저에게 브로드캐스트 알림 처리도 가능
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


