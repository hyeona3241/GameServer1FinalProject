using System;
using NetworkUtil;

public abstract class ChatPacket : Packet
{
    public uint UID { get; set; }
    public ulong Timestamp { get; set; }

    protected ChatPacket()
    {
        UID = 0;
        Timestamp = 0;
    }

    protected ChatPacket(uint uid, ulong timestamp)
    {
        UID = uid;
        Timestamp = timestamp;
    }

    public override bool IsValid()
    {
        return UID != 0 && Timestamp > 0;
    }

    public static ulong GetCurrentUnixTimestamp()
    {
        return (ulong)DateTimeOffset.UtcNow.ToUnixTimeSeconds();
    }
}
