namespace NetworkUtil
{
    public enum EPacketType : ushort
    {
        LOGIN_NICKNAME_REQ = 1001,
        LOGIN_NICKNAME_ACK = 1002,
        LOGIN_MODEL_SELECT_REQ = 1003,
        LOGIN_MODEL_SELECT_ACK = 1004,
        LOGIN_FINALIZE_REQ = 1005,
        LOGIN_SUCCESS_ACK = 1006,
        LOGOUT_REQ = 1007,

        CHAT_CHANNEL_ENTER_REQ = 2001,
        CHAT_CHANNEL_ENTER_ACK = 2002,
        CHAT_SEND_REQ = 2003,
        CHAT_BROADCAST = 2004,
        CHAT_CHANNEL_LEAVE_REQ = 2005,

        ERROR_MSG = 9001
    }
}