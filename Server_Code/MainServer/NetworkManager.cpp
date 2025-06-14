#include "NetworkManager.h"
#include "SessionManager.h"
#include "PacketHandler.h"
#include "Packet.h"
#include "ErrorPacket.h"

// �̱��� �ν��Ͻ� ��ȯ
NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

// ���� ����: ���� �ʱ�ȭ �� ���� ����
bool NetworkManager::StartServer(uint16_t port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return false;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(listenSocket);
        return false;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(listenSocket);
        return false;
    }

    isRunning = true;
    std::thread acceptThread(&NetworkManager::AcceptLoop, this);
    acceptThread.detach(); // ���� ������ ����
    std::cout << "[Server] Listening on port " << port << "...\n";
    return true;
}

// Ŭ���̾�Ʈ ���� ���� ����
void NetworkManager::AcceptLoop()
{
    while (isRunning)
    {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "[Server] Accept failed.\n";
            continue;
        }

        std::cout << "[Server] Client connected.\n";

        std::lock_guard<std::mutex> lock(threadMutex);
        clientThreads.emplace_back(&NetworkManager::ClientWorker, this, clientSocket);
    }
}

// Ŭ���̾�Ʈ ���� ������: ���� �� ó�� �ݺ�
void NetworkManager::ClientWorker(SOCKET clientSocket)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (isRunning)
    {
        int recvLen = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvLen <= 0)
        {
            std::cout << "[Client] Disconnected.\n";
            SessionManager::GetInstance().RemoveUser(clientSocket);
            closesocket(clientSocket);
            break;
        }

        try {
            BinaryReader reader(buffer, recvLen);
            PacketHeader header = reader.ReadHeader();
            PacketHandler::HandlePacket(clientSocket, header, reader);
        }
        catch (const std::length_error& e) {
            std::cerr << "[Client] Length error: " << e.what() << std::endl;

            ErrorPacket error(EErrorCode::INVALID_MODEL_ID, "�߸��� �� ID�Դϴ�.");
            auto data = error.Serialize();
            SendPacket(clientSocket, data);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[Client] Invalid argument: " << e.what() << std::endl;

            ErrorPacket error(EErrorCode::INVALID_NICKNAME, "�г��� ������ �ùٸ��� �ʽ��ϴ�.");
            auto data = error.Serialize();
            SendPacket(clientSocket, data);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "[Client] Runtime error: " << e.what() << std::endl;

            ErrorPacket error(EErrorCode::LOGIN_FAILED, "��û ó�� �� ������ �߻��߽��ϴ�.");
            auto data = error.Serialize();
            SendPacket(clientSocket, data);
        }
        catch (...) {
            std::cerr << "[Client] Unknown error occurred.\n";

            ErrorPacket error(EErrorCode::UNKNOWN_ERROR, "�� �� ���� ������ �߻��߽��ϴ�.");
            auto data = error.Serialize();
            SendPacket(clientSocket, data);
        }
    }
}


// Ŭ���̾�Ʈ���� ������ ����
bool NetworkManager::SendPacket(SOCKET clientSocket, const std::vector<char>& data)
{
    int totalSent = 0;
    int dataSize = static_cast<int>(data.size());

    while (totalSent < dataSize)
    {
        int sent = send(clientSocket, data.data() + totalSent, dataSize - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "[Send] Failed to send data to client.\n";
            return false;
        }
        totalSent += sent;
    }
    return true;
}

// ���� ����: ��� ������ �� ���� ����
void NetworkManager::Shutdown()
{
    isRunning = false;

    if (listenSocket != INVALID_SOCKET) {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }

    {
        std::lock_guard<std::mutex> lock(threadMutex);
        for (std::thread& t : clientThreads) {
            if (t.joinable())
                t.join();
        }
        clientThreads.clear();
    }

    WSACleanup();
    std::cout << "[Server] Shutdown complete.\n";
}
