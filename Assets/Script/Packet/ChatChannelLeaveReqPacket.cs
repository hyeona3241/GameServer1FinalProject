using System;
using NetworkUtil;

public class ChatSendReqPacket : ChatPacket
{
    public override EPacketType PacketType => EPacketType.CHAT_SEND_REQ;

    public string Message { get; private set; }

    public ChatSendReqPacket() : base() { }

    public ChatSendReqPacket(uint uid, ulong timestamp, string message)
        : base(uid, timestamp)
    {
        this.Message = message;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt32(UID);
        writer.WriteUInt64(Timestamp);
        writer.WriteString(Message);
    }

    public override void Deserialize(BinaryReader reader)
    {
        UID = reader.ReadUInt32();
        Timestamp = reader.ReadUInt64();
        Message = reader.ReadString();
    }
}