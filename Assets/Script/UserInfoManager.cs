using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserInfoManager : MonoBehaviour
{
    private static UserInfoManager instance;
    public static UserInfoManager Instance => instance;

    public string Nickname { get; private set; }
    public uint UID { get; private set; }
    public byte ModelID { get; private set; }

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject); // 씬 전환에도 유지
        }
        else
        {
            Destroy(gameObject);
        }
    }

    public void SetUserInfo(string nickname, uint uid)
    {
        Nickname = nickname;
        UID = uid;
    }

    public void SetModle(byte modelId)
    {
        ModelID = modelId;
    }
}
