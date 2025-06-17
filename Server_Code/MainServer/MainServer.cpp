#include "NetworkManager.h"

int main()
{
    const uint16_t SERVER_PORT = 7777;

    // ���� ����
    if (!NetworkManager::GetInstance().StartServer(SERVER_PORT)) {
        std::cerr << "[Main] ���� ���� ����.\n";
        return -1;
    }

    std::cout << "[Main] ������ ���� ���Դϴ�. �����Ϸ��� Enter Ű�� ��������.\n";
    std::cin.get();  // ���� ���

    // ���� ����
    NetworkManager::GetInstance().Shutdown();

    return 0;
}
