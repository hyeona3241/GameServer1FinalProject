using UnityEngine;
using TMPro;

public class LobbyUIController : MonoBehaviour
{
    public TMP_Text NicknameText;
    public TMP_Text UIDText;
    public TMP_Text ModelIdText;

    private void OnEnable()
    {
        var client = UserInfoManager.Instance;

        NicknameText.text = $"Nickname: {client.Nickname}";
        UIDText.text = $"UID: {client.UID}";
        ModelIdText.text = $"ModelID: {client.ModelID}";
    }

    void Start()
    {
        var client = UserInfoManager.Instance;

        NicknameText.text = $"Nickname: {client.Nickname}";
        UIDText.text = $"UID: {client.UID}";
        ModelIdText.text = $"ModelID: {client.ModelID}";
    }
}