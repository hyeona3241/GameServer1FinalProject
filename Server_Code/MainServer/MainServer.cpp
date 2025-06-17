#include "NetworkManager.h"

int main()
{
    const uint16_t SERVER_PORT = 7777;

    // 서버 시작
    if (!NetworkManager::GetInstance().StartServer(SERVER_PORT)) {
        std::cerr << "[Main] 서버 시작 실패.\n";
        return -1;
    }

    std::cout << "[Main] 서버가 실행 중입니다. 종료하려면 Enter 키를 누르세요.\n";
    std::cin.get();  // 종료 대기

    // 서버 종료
    NetworkManager::GetInstance().Shutdown();

    return 0;
}
