using System;
using NetworkUtil;

public abstract class Packet
{
    public abstract EPacketType PacketType { get; }

    public abstract void Serialize(BinaryWriter writer);
    public abstract void Deserialize(BinaryReader reader);

    public abstract bool IsValid();
}
