using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

// ���� ������ ä�� UI�� �����ϴ� Ŭ�����Դϴ�.
// ����ڰ� ä�� �Է� �� ������ �޽����� ������, ���ŵ� �޽����� ȭ�鿡 ����մϴ�.
// �Էµ� ��ɾ�(`/logout`)�� ���� �α׾ƿ� ó���� �����մϴ�.

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

            Canvas.ForceUpdateCanvases(); // ���̾ƿ� ���� ����
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
            Debug.LogWarning("[Chat] �Էµ� �޽����� �����ϴ�.");
            return;
        }

        string rawMessage = inputField.text.Trim();
        inputField.text = ""; // �Է� �ʵ� �ʱ�ȭ

        if (rawMessage.Equals("/Logout", StringComparison.OrdinalIgnoreCase))
        {
            HandleLogoutCommand();
            return;
        }

        uint uid = UserInfoManager.Instance.UID;
        string nickname = UserInfoManager.Instance.Nickname;
        ulong timestamp = (ulong)System.DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();

        // �ð� ���� ��: (14:32)
        string timeStr = DateTime.Now.ToString("HH:mm");
        string finalMessage = $"[{nickname}] : {rawMessage} ({timeStr})";

        var packet = new ChatSendReqPacket(uid, timestamp, finalMessage);
        networkClient.SendPacket(packet);

        Debug.Log($"[Chat] SendPacket: {finalMessage}");
    }

    private void HandleLogoutCommand()
    {
        Debug.Log("[Chat] �α׾ƿ� ��ɾ� �����.");

        uint uid = UserInfoManager.Instance.UID;

        var logoutPacket = new LogoutReqPacket(uid);
        networkClient.SendPacket(logoutPacket);

        Debug.Log("[Chat] �α׾ƿ� ��Ŷ ���� �Ϸ�. Ŭ���̾�Ʈ�� �����մϴ�.");

        // �ణ�� ������ �� ���� (��Ŷ ���� �����)
        StartCoroutine(QuitAfterDelay(0.1f));
    }

    private IEnumerator QuitAfterDelay(float seconds)
    {
        yield return new WaitForSeconds(seconds);
        Application.Quit();

    #if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false; // ������ ���� ���� ��� ����
    #endif
    }

}
