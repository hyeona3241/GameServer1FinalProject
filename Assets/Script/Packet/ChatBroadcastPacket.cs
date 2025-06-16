using System;
using NetworkUtil;

public class ChatBroadcastPacket : ChatPacket
{
    public string Message { get; private set; }

    public override EPacketType PacketType => EPacketType.CHAT_BROADCAST;

    public ChatBroadcastPacket() : base()
    {
        Message = string.Empty;
    }

    public ChatBroadcastPacket(uint uid, ulong timestamp, string message)
        : base(uid, timestamp)
    {
        Message = message;
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
