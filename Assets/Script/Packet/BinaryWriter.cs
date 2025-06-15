using System;
using System.Collections.Generic;
using System.Text;

namespace NetworkUtil
{
    public class BinaryWriter
    {
        private List<byte> buffer = new List<byte>();

        public void WriteUInt8(byte value) => buffer.Add(value);

        public void WriteUInt16(ushort value)
        {
            buffer.AddRange(BitConverter.GetBytes(value));
        }

        public void WriteUInt32(uint value)
        {
            buffer.AddRange(BitConverter.GetBytes(value));
        }

        public void WriteInt32(int value)
        {
            buffer.AddRange(BitConverter.GetBytes(value));
        }

        public void WriteFloat(float value)
        {
            buffer.AddRange(BitConverter.GetBytes(value));
        }

        public void WriteString(string str)
        {
            byte[] strBytes = Encoding.UTF8.GetBytes(str);
            ushort len = (ushort)strBytes.Length;
            WriteUInt16(len);
            buffer.AddRange(strBytes);
        }


        public void WriteHeader(EPacketType type, ushort size)
        {
            WriteUInt16((ushort)type);
            WriteUInt16(size);
        }

        public void WriteBytes(byte[] data)
        {
            if (data == null)
                throw new ArgumentNullException(nameof(data));

            buffer.AddRange(data);
        }

        public byte[] ToArray() => buffer.ToArray();
    }
}