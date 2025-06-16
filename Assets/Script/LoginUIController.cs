using UnityEngine;
using UnityEngine.UI;
using System.IO;
using TMPro;
using UnityEngine.SceneManagement;
using System;

public class LoginUIController : MonoBehaviour
{
    public Button LoginButton;
    public TMP_InputField NickNameInputField;
    public TMP_Text LoginLogText;

    public Button Model1Button;
    public Button Model2Button;
    public Button Model3Button;

    public GameObject LoginSuccessCanvas;
    public GameObject LoginCanvas;
    public GameObject LobbyCanvas;

    public LobbyUIController lobbyUIController;

    private UnityNetworkClient networkClient;

    void Start()
    {
        networkClient = FindObjectOfType<UnityNetworkClient>();

        if (LoginButton != null)
        {
            LoginButton.onClick.AddListener(OnLoginButtonClicked);
        }

        if (Model1Button != null)
            Model1Button.onClick.AddListener(() => OnModelSelectClicked(1));
        
        if (Model2Button != null)
            Model2Button.onClick.AddListener(() => OnModelSelectClicked(2));

        if (Model3Button != null)
            Model3Button.onClick.AddListener(() => OnModelSelectClicked(3));
        

        if (LoginSuccessCanvas != null)
            LoginSuccessCanvas.SetActive(false);

        if (LobbyCanvas != null)
            LobbyCanvas.SetActive(false);
    }

    void OnLoginButtonClicked()
    {
        string nickname = NickNameInputField.text.Trim();

        if (string.IsNullOrEmpty(nickname))
        {
            SetLog("Please enter your nickname.");
            return;
        }

        if (nickname.Length > 16)
        {
            SetLog("Please enter a nickname of 16 characters or less.");
            return;
        }

        SetLog($"Requesting login with nickname \"{nickname}\"...");

        var loginPacket = new LoginNicknameReqPacket(nickname);
        networkClient.SendPacket(loginPacket);
    }

    void SetLog(string message)
    {
        if (LoginLogText != null)
        {
            LoginLogText.text = message;
        }
        else
        {
            Debug.LogWarning("[LoginUIController] LoginLogText is not assigned.");
        }
    }

    public void OnLoginAckReceived(LoginNicknameAckPacket ack)
    {
        if (ack.IsDuplicate)
        {
            SetLog("Failed: Duplicate nickname");
        }
        else
        {
            SetLog($"Available nicknames");

            if (LoginSuccessCanvas != null)
                LoginSuccessCanvas.SetActive(true);
        }
    }

    void OnModelSelectClicked(byte modelId)
    {
        string nickname = NickNameInputField.text.Trim();

        if (string.IsNullOrEmpty(nickname))
        {
            SetLog("Nickname is required to select a model.");
            return;
        }

        SetLog($"Selected model {modelId}. Sending selection...");

        UserInfoManager.Instance.SetModle(modelId);

        var packet = new LoginModelSelectReqPacket(modelId, nickname);
        networkClient.SendPacket(packet);
    }

    public void OnLoginSuccess(LoginSuccessAckPacket ack)
    {
        UserInfoManager.Instance.SetUserInfo(
                               ack.Nickname,
                               ack.UID
        );

        Debug.Log("[Client] 로그인 성공. 메인 씬으로 이동합니다.");

        LoginCanvas.SetActive(false);
        LoginSuccessCanvas.SetActive(false);

        LobbyCanvas.SetActive(true);

        SceneManager.LoadScene("MainScene");

        //채팅 서버 입장 패킷 보내기
        uint uid = UserInfoManager.Instance.UID;
        ulong timestamp = (ulong)DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
        var packet = new ChatChannelEnterReqPacket(uid, timestamp);
        networkClient.SendPacket(packet);

    }
}
