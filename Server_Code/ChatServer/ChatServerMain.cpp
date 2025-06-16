#include "ChatServer.h"

int main() {
    ChatServer& server = ChatServer::GetInstance(); // �̱��� �ν��Ͻ� ȹ��
    if (server.Start(CHAT_SERVER_IP, CHAT_SERVER_PORT)) {
        std::cout << "[ChatServer] Started. Waiting for main server..." << std::endl;
        server.Run(); // ���� ���� ���� (���� �񵿱�� ���� Ȯ�� ����)
    }
    server.Shutdown();
    return 0;
}