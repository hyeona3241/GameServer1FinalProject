using System;
using NetworkUtil;

/// <summary>
/// ������ Ŭ���̾�Ʈ���� �г��� �ߺ� ���ο� UID�� �����ϴ� ��Ŷ.
/// </summary>
public class LoginNicknameAckPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_NICKNAME_ACK;

    public bool IsDuplicate { get; set; }
    public uint UID { get; set; }

    public LoginNicknameAckPacket() : base("") { }

    public LoginNicknameAckPacket(bool isDuplicate, uint uid, string nickname = "") : base(nickname)
    {
        IsDuplicate = isDuplicate;
        UID = isDuplicate ? 0 : uid;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt8((byte)(IsDuplicate ? 1 : 0));

        if (!IsDuplicate)
        {
            writer.WriteUInt32(UID);
        }
    }

    public override void Deserialize(BinaryReader reader)
    {
        IsDuplicate = reader.ReadUInt8() != 0;

        if (!IsDuplicate)
        {
            UID = reader.ReadUInt32();

           
        }
        else
        {
            UID = 0;
        }
    }

    public override bool IsValid()
    {
        if (IsDuplicate)
            return true;

        return UID != 0;
    }

}
