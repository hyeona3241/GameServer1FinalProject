using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

// 메인 씬에서 채팅 UI를 제어하는 클래스입니다.
// 사용자가 채팅 입력 시 서버에 메시지를 보내고, 수신된 메시지를 화면에 출력합니다.
// 입력된 명령어(`/logout`)에 따라 로그아웃 처리도 수행합니다.

public class ChatUIController : MonoBehaviour
{
    public TMP_Text chatTextArea;
    public TMP_InputField inputField;
    public Button sendButton;
    public ScrollRect scrollRect;

    private UnityNetworkClient networkClient;

    void Start()
    {
        networkClient = FindObjectOfType<UnityNetworkClient>();

        if (sendButton != null)
        {
            sendButton.onClick.AddListener(OnSendButtonClicked);
        }
    }


    public void AppendMessage(uint uid, string message)
    {
        if (chatTextArea != null)
        {
            string formatted = $"[{uid}] {message}\n";
            chatTextArea.text += formatted;

            Canvas.ForceUpdateCanvases(); // 레이아웃 강제 갱신
            scrollRect.verticalNormalizedPosition = 0f;
        }
        else
        {
            Debug.LogWarning("chatTextArea is not assigned.");
        }
    }

    public void OnSendButtonClicked()
    {
        if (inputField == null || string.IsNullOrWhiteSpace(inputField.text))
        {
            Debug.LogWarning("[Chat] 입력된 메시지가 없습니다.");
            return;
        }

        string rawMessage = inputField.text.Trim();
        inputField.text = ""; // 입력 필드 초기화

        if (rawMessage.Equals("/Logout", StringComparison.OrdinalIgnoreCase))
        {
            HandleLogoutCommand();
            return;
        }

        uint uid = UserInfoManager.Instance.UID;
        string nickname = UserInfoManager.Instance.Nickname;
        ulong timestamp = (ulong)System.DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();

        // 시간 포맷 예: (14:32)
        string timeStr = DateTime.Now.ToString("HH:mm");
        string finalMessage = $"[{nickname}] : {rawMessage} ({timeStr})";

        var packet = new ChatSendReqPacket(uid, timestamp, finalMessage);
        networkClient.SendPacket(packet);

        Debug.Log($"[Chat] SendPacket: {finalMessage}");
    }

    private void HandleLogoutCommand()
    {
        Debug.Log("[Chat] 로그아웃 명령어 실행됨.");

        uint uid = UserInfoManager.Instance.UID;

        var logoutPacket = new LogoutReqPacket(uid);
        networkClient.SendPacket(logoutPacket);

        Debug.Log("[Chat] 로그아웃 패킷 전송 완료. 클라이언트를 종료합니다.");

        // 약간의 딜레이 후 종료 (패킷 전송 보장용)
        StartCoroutine(QuitAfterDelay(0.1f));
    }

    private IEnumerator QuitAfterDelay(float seconds)
    {
        yield return new WaitForSeconds(seconds);
        Application.Quit();

    #if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false; // 에디터 실행 중일 경우 종료
    #endif
    }

}
