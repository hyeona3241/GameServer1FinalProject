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
        LoginNicknameReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        const std::string& nickname = req.GetNickname();

        LoginNicknameAckPacket ack;

        if (!SessionManager::GetInstance().IsNicknameDuplicate(nickname)) {
            // 닉네임이 이미 존재하는 경우 → 중복 플래그 true
            ack.SetIsDuplicate(true);
        }
        else {
            // 사용 가능한 닉네임 → 중복 플래그 false, UID는 아직 부여하지 않음
            ack.SetIsDuplicate(false);
            ack.SetUID(0);  // 규칙상 UID는 나중에 부여
        }

        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleLoginNicknameReq] Exception: " << e.what() << std::endl;

        // 클라가 에러 패킷이 아닌 일반 응답 패킷을 기다리므로, 에러 패킷은 보내지 않음.
        // 로그만 남기고 무시 or 종료
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
                isValid = false; // 유저 세션이 없으면 유효성 false로 응답
            }
        }

        LoginModelSelectAckPacket ack(isValid, modelId);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());
    }
    catch (const std::exception& e) {
        std::cerr << "[HandleModelSelect] Exception: " << e.what() << std::endl;
        // 클라가 ACK 패킷을 기다리므로, 에러 패킷은 생략
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

        // 2. UID 발급 및 등록
        uint32_t uid = SessionManager::GetInstance().RegisterNewUser(*session);

        // 3. 응답 패킷 전송
        LoginSuccessAckPacket ack(uid, session->nickname);
        NetworkManager::GetInstance().SendPacket(clientSocket, ack.Serialize());

        std::cout << "[HandleLoginFinalize] 로그인 완료: UID = " << uid
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
        LogoutReqPacket req;
        req.Deserialize(reader.GetBuffer(), reader.GetBufferSize());

        uint32_t uid = req.GetUID();

        // 세션 제거 시도
        bool success = SessionManager::GetInstance().RemoveUser(uid);

        if (success) {
            std::cout << "[HandleLogoutReq] 유저 로그아웃 성공 - UID: " << uid << std::endl;

            // 필요 시: 다른 유저에게 브로드캐스트 알림 처리도 가능
        }
        else {
            std::cerr << "[HandleLogoutReq] 유저 정보 삭제 실패 - UID: " << uid << std::endl;

            // 클라에 에러 패킷 보내는 것도 고려 가능
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

