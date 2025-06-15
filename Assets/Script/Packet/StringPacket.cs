using System;
using System.Text;
using NetworkUtil;

public abstract class StringPacket : Packet
{
    public string Data { get; set; }

    protected StringPacket(string data = "")
    {
        if (Encoding.UTF8.GetByteCount(data) > 1024)
            throw new ArgumentException("StringPacket: data too long (max 1024 bytes UTF-8)");
        Data = data;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteString(Data);
    }

    public override void Deserialize(BinaryReader reader)
    {
        Data = reader.ReadString();
        if (Encoding.UTF8.GetByteCount(Data) > 1024)
            throw new InvalidOperationException("StringPacket: Deserialized data too long");
    }

    public override bool IsValid()
    {
        return !string.IsNullOrEmpty(Data) && Encoding.UTF8.GetByteCount(Data) <= 1024;
    }
}
