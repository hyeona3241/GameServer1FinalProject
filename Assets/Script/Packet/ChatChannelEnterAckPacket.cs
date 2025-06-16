using System;
using NetworkUtil;

public class ChatChannelEnterAckPacket : ChatPacket
{
    public override EPacketType PacketType => EPacketType.CHAT_CHANNEL_ENTER_ACK;

    public ChatChannelEnterAckPacket() : base() { }

    public ChatChannelEnterAckPacket(uint uid, ulong timestamp)
        : base(uid, timestamp) { }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt32(UID);
        writer.WriteUInt64(Timestamp);
    }

    public override void Deserialize(BinaryReader reader)
    {
        UID = reader.ReadUInt32();
        Timestamp = reader.ReadUInt64();
    }

}
