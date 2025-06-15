using System;
using System.Text;
using NetworkUtil;

public class LoginModelSelectReqPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_MODEL_SELECT_REQ;

    public byte ModelId { get; private set; }

    public LoginModelSelectReqPacket() { }

    public LoginModelSelectReqPacket(byte modelId, string nickname) : base(nickname)
    {
        ModelId = modelId;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt8(ModelId);
        writer.WriteString(Nickname); // UTF-8, 길이 포함
    }

    public override void Deserialize(BinaryReader reader)
    {
        ModelId = reader.ReadUInt8();
        if (ModelId > 3)
            throw new ArgumentException("Invalid model ID");

        Nickname = reader.ReadString();
    }

    public override bool IsValid()
    {
        return ModelId <= 3 && !string.IsNullOrEmpty(Nickname) && Encoding.UTF8.GetByteCount(Nickname) <= 255;
    }
}
