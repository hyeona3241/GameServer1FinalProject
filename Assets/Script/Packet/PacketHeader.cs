namespace NetworkUtil
{
    public struct PacketHeader
    {
        public EPacketType type;
        public ushort size;

        public PacketHeader(EPacketType type, ushort size)
        {
            this.type = type;
            this.size = size;
        }
    }
}