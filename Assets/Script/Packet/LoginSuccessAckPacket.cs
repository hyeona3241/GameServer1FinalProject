using System;
using System.Text;
using NetworkUtil;

/// <summary>
/// ������ Ŭ���̾�Ʈ���� �α��� ���� ��ǰ� UID, �г����� �����ϴ� ��Ŷ
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
