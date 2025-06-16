using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;


public class ChatUIController : MonoBehaviour
{
    public TMP_Text chatTextArea;

    public void AppendMessage(uint uid, string message)
    {
        if (chatTextArea != null)
        {
            string formatted = $"[{uid}] {message}\n";
            chatTextArea.text += formatted;
        }
        else
        {
            Debug.LogWarning("chatTextArea is not assigned.");
        }
    }
}
