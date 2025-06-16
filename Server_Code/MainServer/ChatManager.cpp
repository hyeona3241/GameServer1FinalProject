#include "ChatManager.h"
#include "PacketHandler.h"
#include "BinaryReader.h"

ChatManager::ChatManager() {}

ChatManager::~ChatManager() {
    Disconnect();
}

ChatManager& ChatManager::GetInstance() {
    static ChatManager instance;
    return instance;
}

bool ChatManager::ConnectToChatServer(const std::string& ip, uint16_t port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[ChatManager] WSAStartup failed." << std::endl;
        return false;
    }

    chatSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (chatSocket == INVALID_SOCKET) {
        std::cerr << "[ChatManager] socket() failed." << std::endl;
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(chatSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[ChatManager] connect() failed." << std::endl;
        closesocket(chatSocket);
        WSACleanup();
        return false;
    }

    std::cout << "[ChatManager] Connected to Chat Server at " << ip << ":" << port << std::endl;

    StartReceiveLoop(); // ���� �� ���� ���� ����
    return true;
}

void ChatManager::Disconnect()
{
    StopReceiveLoop();

    if (chatSocket != INVALID_SOCKET) {
        closesocket(chatSocket);
        chatSocket = INVALID_SOCKET;
    }

    WSACleanup();
    std::cout << "[ChatManager] Disconnected from Chat Server." << std::endl;
}

void ChatManager::SendPacket(const Packet& packet)
{
    if (chatSocket == INVALID_SOCKET) {
        std::cerr << "[ChatManager::SendPacket] ��ȿ���� ���� �����Դϴ�." << std::endl;
        return;
    }

    auto buffer = packet.Serialize();

    std::lock_guard<std::mutex> lock(sendMutex);
    int sent = send(chatSocket, reinterpret_cast<const char*>(buffer.data()), static_cast<int>(buffer.size()), 0);

    if (sent == SOCKET_ERROR) {
        std::cerr << "[ChatManager::SendPacket] ���� ���� - ���� �ڵ�: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "[ChatManager::SendPacket] ��Ŷ ���� ���� - Type: " << static_cast<uint16_t>(packet.GetType())
            << ", Size: " << buffer.size() << " bytes" << std::endl;
    }
}


void ChatManager::StartReceiveLoop()
{
    isReceiving = true;
    recvThread = std::thread(&ChatManager::ReceiveLoop, this);
}

void ChatManager::StopReceiveLoop()
{
    isReceiving = false;
    if (recvThread.joinable()) {
        recvThread.join();
    }
}

void ChatManager::ReceiveLoop()
{
    constexpr int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    while (isReceiving)
    {
        int received = recv(chatSocket, buffer, BUFFER_SIZE, 0);
        if (received <= 0) {
            std::cerr << "[ChatManager] ä�� ���� ���� ���� or ���� ����" << std::endl;
            break;
        }

        // �ּ��� 4����Ʈ(����) + Header�� �־�� ���� ��Ŷ���� ó�� ����
        if (received < sizeof(uint32_t) + sizeof(PacketHeader)) {
            std::cerr << "[ChatManager] ���� �����Ͱ� �ʹ� ���� (" << received << " bytes)" << std::endl;
            continue;
        }

        // 1. ���� �Ľ�
        uint32_t packetSize = 0;
        memcpy(&packetSize, buffer, sizeof(uint32_t));

        // 2. ���ŵ� �����Ͱ� ���̺��� ������ ����
        if (received < static_cast<int>(packetSize + sizeof(uint32_t))) {
            std::cerr << "[ChatManager] ���ŵ� ��Ŷ ũ�� ����: ���=" << (packetSize + 4)
                << ", ����=" << received << std::endl;
            continue;
        }

        // 3. BinaryReader ���� (payload ���� ��ġ�� buffer + 4)
        BinaryReader reader(buffer + sizeof(uint32_t), packetSize);
        PacketHeader header = reader.ReadHeader();

        // 4. �ڵ鷯 ȣ��
        PacketHandler::HandleChatPacket(chatSocket, header, reader);
    }

    Disconnect();
}


void ChatManager::AddChatUser(uint32_t uid, const std::string& nickname, SOCKET socket)
{
    std::lock_guard<std::mutex> lock(userMutex);
    chatUserMap[uid] = {uid, nickname, socket };
    chatNicknameMap[nickname] = uid;

    std::cout << "[ChatManager] ä�� ���� �߰�: UID=" << uid << ", �г���=" << nickname << std::endl;
}

void ChatManager::RemoveChatUser(uint32_t uid)
{
    std::lock_guard<std::mutex> lock(userMutex);
    auto it = chatUserMap.find(uid);
    if (it != chatUserMap.end()) {
        chatNicknameMap.erase(it->second.nickname);  // �г��� ��ݵ� ����
        chatUserMap.erase(it);
        std::cout << "[ChatManager] ä�� ���� ����: UID=" << uid << std::endl;
    }
}

bool ChatManager::IsUserInChat(uint32_t uid) const
{
    std::lock_guard<std::mutex> lock(userMutex);
    return chatUserMap.find(uid) != chatUserMap.end();
}

std::string ChatManager::GetNicknameByUID(uint32_t uid) const
{
    std::lock_guard<std::mutex> lock(userMutex);
    auto it = chatUserMap.find(uid);
    return (it != chatUserMap.end()) ? it->second.nickname : "";
}

uint32_t ChatManager::GetUIDByNickname(const std::string& nickname) const
{
    std::lock_guard<std::mutex> lock(userMutex);
    auto it = chatNicknameMap.find(nickname);
    return (it != chatNicknameMap.end()) ? it->second : 0;
}

SOCKET ChatManager::GetSocketByUID(uint32_t uid) const
{
    std::lock_guard<std::mutex> lock(userMutex);
    auto it = chatUserMap.find(uid);
    return (it != chatUserMap.end()) ? it->second.socket : INVALID_SOCKET;
}

std::vector<SOCKET> ChatManager::GetAllClientSockets() const
{
    std::lock_guard<std::mutex> lock(userMutex);
    std::vector<SOCKET> sockets;
    for (const auto& pair : chatUserMap) {
        sockets.push_back(pair.second.socket);
    }
    return sockets;
}

