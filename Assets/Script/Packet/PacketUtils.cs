using System;
using System.IO;
using System.Text;

public static class PacketUtils
{
    // ��Ŷ ��� �ۼ�
    public static void WriteHeader(BinaryWriter writer, EPacketType type, ushort size)
    {
        writer.Write((ushort)type);
        writer.Write(size);
    }

    // ��Ŷ ��� �б�
    public static PacketHeader ReadHeader(BinaryReader reader)
    {
        EPacketType type = (EPacketType)reader.ReadUInt16();
        ushort size = reader.ReadUInt16();
        return new PacketHeader(type, size);
    }

    // ���ڿ� ���� (���� ��� ����ȭ)
    public static void WriteString(BinaryWriter writer, string value)
    {
        byte[] strBytes = Encoding.UTF8.GetBytes(value);
        if (strBytes.Length > 255)
            throw new ArgumentException("String too long to serialize");

        writer.Write((byte)strBytes.Length); // ���ڿ� ���� 1����Ʈ
        writer.Write(strBytes);              // ���ڿ� ����
    }

    // ���ڿ� �б�
    public static string ReadString(BinaryReader reader)
    {
        byte length = reader.ReadByte();             // ���� �б�
        byte[] strBytes = reader.ReadBytes(length);  // ���� �б�
        return Encoding.UTF8.GetString(strBytes);
    }
}

public class PacketWriter
{
    private MemoryStream stream = new MemoryStream();
    private BinaryWriter writer;

    public PacketWriter()
    {
        writer = new BinaryWriter(stream, Encoding.UTF8);
    }

    public void WriteHeader(EPacketType type, ushort size)
    {
        PacketUtils.WriteHeader(writer, type, size);
    }

    public void WriteUInt8(byte value) => writer.Write(value);
    public void WriteUInt16(ushort value) => writer.Write(value);
    public void WriteUInt32(uint value) => writer.Write(value);
    public void WriteInt32(int value) => writer.Write(value);
    public void WriteFloat(float value) => writer.Write(value);
    public void WriteString(string value) => PacketUtils.WriteString(writer, value);

    public byte[] ToArray() => stream.ToArray();
    public int Length => (int)stream.Length;
}

public class PacketReader
{
    private BinaryReader reader;

    public PacketReader(byte[] data)
    {
        reader = new BinaryReader(new MemoryStream(data), Encoding.UTF8);
    }

    public PacketHeader ReadHeader() => PacketUtils.ReadHeader(reader);
    public byte ReadUInt8() => reader.ReadByte();
    public ushort ReadUInt16() => reader.ReadUInt16();
    public uint ReadUInt32() => reader.ReadUInt32();
    public int ReadInt32() => reader.ReadInt32();
    public float ReadFloat() => reader.ReadSingle();
    public string ReadString() => PacketUtils.ReadString(reader);
}
