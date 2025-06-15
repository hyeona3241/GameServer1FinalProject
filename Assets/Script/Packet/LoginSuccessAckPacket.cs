using System;
using System.Text;
using NetworkUtil;

/// <summary>
/// 서버가 클라이언트에게 로그인 성공 사실과 UID, 닉네임을 전달하는 패킷
/// </summary>
public class LoginSuccessAckPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_SUCCESS_ACK;

    public uint UID { get; set; }

    public LoginSuccessAckPacket() : base("") { }

    public LoginSuccessAckPacket(uint uid, string nickname) : base(nickname)
    {
        UID = uid;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt32(UID);
        writer.WriteString(Nickname);
    }

    public override void Deserialize(BinaryReader reader)
    {
        UID = reader.ReadUInt32();
        Nickname = reader.ReadString();
    }

    public override bool IsValid()
    {
        return UID > 0 && !string.IsNullOrWhiteSpace(Nickname) && Encoding.UTF8.GetByteCount(Nickname) <= 255;
    }
}
