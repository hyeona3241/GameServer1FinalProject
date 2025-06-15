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

        CHAT_SEND = 2001,
        CHAT_RECV = 2002,

        ERROR_MSG = 9001
    }
}