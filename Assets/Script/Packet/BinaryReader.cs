using System;
using System.Text;

namespace NetworkUtil
{
    public class BinaryReader
    {
        private readonly byte[] buffer;
        private int offset;

        public BinaryReader(byte[] buffer)
        {
            this.buffer = buffer ?? throw new ArgumentNullException(nameof(buffer));
            this.offset = 0;
        }

        public byte ReadUInt8()
        {
            if (offset + 1 > buffer.Length)
                throw new IndexOutOfRangeException("ReadUInt8 buffer overrun");
            return buffer[offset++];
        }

        public ushort ReadUInt16()
        {
            if (offset + 2 > buffer.Length)
                throw new IndexOutOfRangeException("ReadUInt16 buffer overrun");

            ushort value = BitConverter.ToUInt16(buffer, offset);
            offset += 2;
            return value;
        }

        public uint ReadUInt32()
        {
            if (offset + 4 > buffer.Length)
                throw new IndexOutOfRangeException("ReadUInt32 buffer overrun");

            uint value = BitConverter.ToUInt32(buffer, offset);
            offset += 4;
            return value;
        }

        public int ReadInt32()
        {
            if (offset + 4 > buffer.Length)
                throw new IndexOutOfRangeException("ReadInt32 buffer overrun");

            int value = BitConverter.ToInt32(buffer, offset);
            offset += 4;
            return value;
        }

        public float ReadFloat()
        {
            if (offset + 4 > buffer.Length)
                throw new IndexOutOfRangeException("ReadFloat buffer overrun");

            float value = BitConverter.ToSingle(buffer, offset);
            offset += 4;
            return value;
        }

        public string ReadString()
        {
            ushort len = ReadUInt16();

            if (offset + len > buffer.Length)
                throw new IndexOutOfRangeException("ReadString buffer overrun");

            string result = Encoding.UTF8.GetString(buffer, offset, len);
            offset += len;
            return result;
        }

        public (EPacketType type, ushort size) ReadHeader()
        {
            var type = (EPacketType)ReadUInt16();
            var size = ReadUInt16();
            return (type, size);
        }
    }
}