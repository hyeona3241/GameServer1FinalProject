#include "ChatServer.h"

int main() {
    ChatServer& server = ChatServer::GetInstance(); // 싱글톤 인스턴스 획득
    if (server.Start(CHAT_SERVER_IP, CHAT_SERVER_PORT)) {
        std::cout << "[ChatServer] Started. Waiting for main server..." << std::endl;
        server.Run(); // 수신 루프 실행 (차후 비동기로 로직 확장 가능)
    }
    server.Shutdown();
    return 0;
}