#include "ChatServer.h"
#include "BinaryReader.h"
#include "ChatPacketHandler.h"
#include "PacketDef.h"

bool ChatServer::Start(const std::string& ip, uint16_t port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[ChatServer] WSAStartup failed." << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "[ChatServer] Socket creation failed." << std::endl;
        return false;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[ChatServer] Bind failed." << std::endl;
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[ChatServer] Listen failed." << std::endl;
        return false;
    }

    std::cout << "[ChatServer] Listening on " << ip << ":" << port << std::endl;
    isRunning = true;

    recvThread = std::thread(&ChatServer::AcceptLoop, this);
    return true;
}

void ChatServer::Run() {
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void ChatServer::AcceptLoop() {
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    mainServerSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
    if (mainServerSocket == INVALID_SOCKET) {
        std::cerr << "[ChatServer] Accept failed." << std::endl;
        return;
    }

    std::cout << "[ChatServer] Main server connected." << std::endl;
    ReceiveLoop();
}

void ChatServer::ReceiveLoop()
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    while (isRunning)
    {
        int received = recv(mainServerSocket, buffer, BUFFER_SIZE, 0);
        if (received <= 0)
        {
            std::cerr << "[ChatServer] ���� ���� �Ǵ� ���� ����" << std::endl;
            break;
        }

        try
        {
            BinaryReader reader(buffer, received);
            PacketHeader header = reader.ReadHeader();
            ChatPacketHandler::HandlePacket(mainServerSocket, header, reader);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[ChatServer] ��Ŷ ó�� �� ���� �߻�: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[ChatServer] �� �� ���� ���� �߻�" << std::endl;
        }
    }
}

void ChatServer::Shutdown() {
    isRunning = false;
    closesocket(mainServerSocket);
    closesocket(serverSocket);
    WSACleanup();
    if (recvThread.joinable()) recvThread.join();
}

bool ChatServer::SendPacket(const std::vector<char>& data)
{
    int totalSent = 0;
    int dataSize = static_cast<int>(data.size());

    // �� ��Ŷ ũ�⸦ �տ� �߰�
    uint32_t totalSize = static_cast<uint32_t>(data.size());
    std::vector<char> fullPacket(sizeof(uint32_t) + dataSize);
    memcpy(fullPacket.data(), &totalSize, sizeof(uint32_t));
    memcpy(fullPacket.data() + sizeof(uint32_t), data.data(), dataSize);

    // ��Ŷ Ÿ�� ����
    EPacketType packetType = EPacketType::None;
    if (data.size() >= sizeof(PacketHeader)) {
        const PacketHeader* header = reinterpret_cast<const PacketHeader*>(data.data());
        packetType = static_cast<EPacketType>(header->type);
    }

    // send loop
    int fullSize = static_cast<int>(fullPacket.size());
    while (totalSent < fullSize)
    {
        int sent = send(mainServerSocket, fullPacket.data() + totalSent, fullSize - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "[ChatServer] ������ ���� ���� - Ÿ��: " << static_cast<uint16_t>(packetType)
                << ", ���� ����Ʈ: " << totalSent << " / ��ü ũ��: " << fullSize << std::endl;
            return false;
        }
        totalSent += sent;
    }

    std::cout << "[ChatServer] ���� �Ϸ� - Ÿ��: " << static_cast<uint16_t>(packetType)
        << ", �� ����Ʈ: " << totalSent << std::endl;
    return true;
}
