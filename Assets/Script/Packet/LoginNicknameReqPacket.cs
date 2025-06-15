using NetworkUtil;
using System;
using System.Text;

public class LoginNicknameReqPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_NICKNAME_REQ;

    public LoginNicknameReqPacket() { }

    public LoginNicknameReqPacket(string nickname)
        : base(nickname)
    {
        if (!IsValid())
            throw new ArgumentException("Nickname must be 1~255 bytes in UTF-8.");
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteString(Nickname);
    }

    public override void Deserialize(BinaryReader reader)
    {
        Nickname = reader.ReadString();
        if (!IsValid())
            throw new ArgumentException("Nickname must be 1~255 bytes in UTF-8.");
    }
}
