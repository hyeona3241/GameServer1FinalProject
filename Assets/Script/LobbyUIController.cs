using UnityEngine;
using TMPro;

// 로그인 성공 이후, 캐릭터(모델) 선택 씬의 UI를 제어합니다.
// 모델 선택을 서버에 전송하며, 선택 확정 시 메인 씬으로 이동합니다.

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