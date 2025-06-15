using System;
using System.Text;
using NetworkUtil;

public class ErrorPacket : Packet
{
    public override EPacketType PacketType => EPacketType.ERROR_MSG;

    public ushort ErrorCode { get; set; }
    public string Message { get; set; }

    public const int MAX_ERROR_MSG_SIZE = 512;

    public ErrorPacket() { }

    public ErrorPacket(ushort code, string message)
    {
        if (string.IsNullOrEmpty(message) || Encoding.UTF8.GetByteCount(message) > MAX_ERROR_MSG_SIZE)
            throw new ArgumentException("Error message must be 1~512 bytes in UTF-8");

        ErrorCode = code;
        Message = message;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt16(ErrorCode);
        writer.WriteString(Message);
    }

    public override void Deserialize(BinaryReader reader)
    {
        ErrorCode = reader.ReadUInt16();
        Message = reader.ReadString();

        if (string.IsNullOrEmpty(Message) || Encoding.UTF8.GetByteCount(Message) > MAX_ERROR_MSG_SIZE)
            throw new InvalidOperationException("Malformed ErrorPacket message");
    }

    public override bool IsValid()
    {
        return !string.IsNullOrEmpty(Message) && Encoding.UTF8.GetByteCount(Message) <= MAX_ERROR_MSG_SIZE;
    }
}
