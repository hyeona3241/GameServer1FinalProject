using System;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using NetworkUtil;

public class UnityNetworkClient : MonoBehaviour
{
    private TcpClient tcpClient;
    private NetworkStream stream;
    private Thread receiveThread;
    private bool isRunning = false;

    public string serverIP = "127.0.0.1";
    public int serverPort = 7777;

    private LoginUIController loginUIController;
    private ChatUIController chatUIController;

    //메인 스레드에서 실행할 작업 큐
    private readonly Queue<Action> mainThreadActions = new Queue<Action>();
    private readonly object actionLock = new object();

    void Start()
    {
        ConnectToServer();
    }

    void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }

    void Update()
    {
        lock (actionLock)
        {
            while (mainThreadActions.Count > 0)
            {
                var action = mainThreadActions.Dequeue();
                action?.Invoke();
            }
        }

        if (loginUIController == null && UnityEngine.SceneManagement.SceneManager.GetActiveScene().name == "LoginScene")
        {
            loginUIController = FindObjectOfType<LoginUIController>();
        }

        if (loginUIController == null && UnityEngine.SceneManagement.SceneManager.GetActiveScene().name == "MainScene")
        {
            chatUIController = FindObjectOfType<ChatUIController>();
        }
    }

    void ConnectToServer()
    {
        try
        {
            tcpClient = new TcpClient(serverIP, serverPort);
            stream = tcpClient.GetStream();
            isRunning = true;

            receiveThread = new Thread(ReceiveLoop);
            receiveThread.Start();

            Debug.Log("Connected to server.");
        }
        catch (Exception e)
        {
            Debug.LogError("Connection failed: " + e.Message);
        }
    }

    public void SendPacket(Packet packet)
    {
        if (stream == null || !stream.CanWrite) return;

        var bodyWriter = new NetworkUtil.BinaryWriter();
        packet.Serialize(bodyWriter);
        byte[] body = bodyWriter.ToArray();

        var finalWriter = new NetworkUtil.BinaryWriter();
        finalWriter.WriteHeader(packet.PacketType, (ushort)(body.Length + 4));
        finalWriter.WriteBytes(body);

        byte[] buffer = finalWriter.ToArray();

        try
        {
            //1. 닉네임 디버그 로그
            if (packet is LoginNicknameReqPacket nickPacket)
            {
                Debug.Log($"[Packet Debug] Nickname: {nickPacket.Nickname}");
                Debug.Log($"[Packet Debug] UTF-8 Byte Length: {System.Text.Encoding.UTF8.GetByteCount(nickPacket.Nickname)}");
            }

            //2. 패킷 전체 바이트 확인
            Debug.Log($"[Packet Debug] Sent Bytes: {BitConverter.ToString(buffer)}");

            //3. 실제 전송
            stream.Write(buffer, 0, buffer.Length);
            stream.Flush();
        }
        catch (Exception e)
        {
            Debug.LogError("SendPacket error: " + e.Message);
        }
    }

    void ReceiveLoop()
    {
        try
        {
            while (isRunning)
            {
                byte[] headerBytes = new byte[4]; // PacketHeader: type(2) + size(2)
                if (!ReadFully(headerBytes, 4)) break;

                ushort typeValue = BitConverter.ToUInt16(headerBytes, 0);
                ushort size = BitConverter.ToUInt16(headerBytes, 2);
                EPacketType type = (EPacketType)typeValue;

                Debug.Log($"[ReceiveLoop] Header Read - Type: {type} ({typeValue}), Size: {size}");

                byte[] bodyBytes = new byte[size - 4];
                if (!ReadFully(bodyBytes, bodyBytes.Length)) break;

                byte[] fullPacket = new byte[size];
                Buffer.BlockCopy(headerBytes, 0, fullPacket, 0, 4);
                Buffer.BlockCopy(bodyBytes, 0, fullPacket, 4, bodyBytes.Length);

                HandleIncomingPacket(fullPacket);
            }
        }
        catch (Exception e)
        {
            Debug.LogError($"[ReceiveLoop] Exception Type: {e.GetType().Name}");
            Debug.LogError($"[ReceiveLoop] Exception Message: {e.Message}");
            Debug.LogError($"[ReceiveLoop] Stack Trace:\n{e.StackTrace}");

        }
    }

    bool ReadFully(byte[] buffer, int length)
    {
        int offset = 0;
        while (offset < length)
        {
            int read = stream.Read(buffer, offset, length - offset);
            if (read <= 0) return false;
            offset += read;
        }
        return true;
    }

    void HandleIncomingPacket(byte[] data)
    {
        var reader = new NetworkUtil.BinaryReader(data);

        EPacketType type = (EPacketType)reader.ReadUInt16();
        ushort size = reader.ReadUInt16();

        try
        {
            switch (type)
            {
                case EPacketType.ERROR_MSG:
                    {
                        var error = new ErrorPacket();
                        error.Deserialize(reader);
                        EnqueueMainThreadAction(() =>
                        {
                            Debug.LogWarning($"[Server Error] {error.ErrorCode} : {error.Message}");
                        });
                        break;
                    }

                case EPacketType.LOGIN_SUCCESS_ACK:
                    {
                        var loginAck = new LoginSuccessAckPacket();
                        loginAck.Deserialize(reader);

                        EnqueueMainThreadAction(() =>
                        {
                            Debug.Log($"[Login Success] UID: {loginAck.UID}, Nick: {loginAck.Nickname}");

                            loginUIController?.OnLoginSuccess(loginAck);

                        });
                        break;
                    }

                case EPacketType.LOGIN_NICKNAME_ACK:
                    {
                        var ack = new LoginNicknameAckPacket();
                        ack.Deserialize(reader);

                        Debug.Log($"[Packet Received] LOGIN_NICKNAME_ACK | IsDuplicate: {ack.IsDuplicate}, UID: {ack.UID}");

                        EnqueueMainThreadAction(() =>
                        {
                            loginUIController?.OnLoginAckReceived(ack);
                        });
                        break;
                    }

                case EPacketType.LOGIN_MODEL_SELECT_ACK:
                    {
                        var ack = new LoginModelSelectAckPacket();
                        ack.Deserialize(reader);

                        Debug.Log($"[Packet Received] LOGIN_MODEL_SELECT_ACK | ModelID: {ack.ModelId}");

                        EnqueueMainThreadAction(() =>
                        {
                            var finalizeReq = new LoginFinalizeReqPacket();
                            SendPacket(finalizeReq);

                            Debug.Log("[Client] Sent LOGIN_FINALIZE_REQ packet.");
                        });
                        break;
                    }

                case EPacketType.CHAT_BROADCAST:
                    {
                        var broadcast = new ChatBroadcastPacket();
                        broadcast.Deserialize(reader);

                        EnqueueMainThreadAction(() =>
                        {
                            Debug.Log($"[Chat] {broadcast.UID}: {broadcast.Message}");

                            var chatUI = FindObjectOfType<ChatUIController>();
                            if (chatUI != null)
                            {
                                chatUI.AppendMessage(broadcast.UID, broadcast.Message);
                            }
                            else
                            {
                                Debug.LogWarning("[Chat] ChatUIController를 찾을 수 없습니다.");
                            }
                        });
                        break;
                    }

                default:
                    Debug.Log($"Unknown packet type: {type}");
                    break;
            }
        }
        catch (Exception e)
        {
            Debug.LogError($"Failed to deserialize packet of type {type}: {e.Message}");
        }
    }

    //메인 스레드 작업 큐에 등록
    private void EnqueueMainThreadAction(Action action)
    {
        lock (actionLock)
        {
            mainThreadActions.Enqueue(action);
        }
    }

    

    private void OnApplicationQuit()
    {
        if (UserInfoManager.Instance.UID != 0 && stream != null && stream.CanWrite)
        {
            try
            {
                uint uid = UserInfoManager.Instance.UID;

                Debug.Log($"[Client] 애플리케이션 종료 전 로그아웃 패킷 전송 중... (UID: {uid})");

                var logoutPacket = new LogoutReqPacket(uid);
                SendPacket(logoutPacket);

                // Optional: 패킷 전송 후 약간의 시간 대기
                Thread.Sleep(50); // 50ms 정도 기다리면 서버가 받기 쉬움
            }
            catch (Exception e)
            {
                Debug.LogWarning($"[Client] 로그아웃 패킷 전송 실패: {e.Message}");
            }
        }

        isRunning = false;

        try
        {
            stream?.Close();
            tcpClient?.Close();
            receiveThread?.Join(); // 안전하게 종료 대기
        }
        catch (Exception e)
        {
            Debug.LogWarning("Shutdown error: " + e.Message);
        }
    }
}
