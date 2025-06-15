using System;
using NetworkUtil;

public class LoginModelSelectAckPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_MODEL_SELECT_ACK;

    public bool IsValidModel { get; set; }
    public byte ModelId { get; set; }

    public LoginModelSelectAckPacket() : base("") { }

    public LoginModelSelectAckPacket(bool isValidModel, byte modelId, string nickname = "") : base(nickname)
    {
        IsValidModel = isValidModel;
        ModelId = modelId;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt8((byte)(IsValidModel ? 1 : 0));
        writer.WriteUInt8(ModelId);
    }

    public override void Deserialize(BinaryReader reader)
    {
        IsValidModel = reader.ReadUInt8() != 0;
        ModelId = reader.ReadUInt8();

        if (IsValidModel && ModelId > 3)
            UnityEngine.Debug.LogWarning($"[Warning] Unexpected model ID received: {ModelId}");
    }

    public override bool IsValid()
    {
        return !IsValidModel || ModelId <= 3;
    }
}
