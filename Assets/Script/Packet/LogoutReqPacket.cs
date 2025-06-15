using System;
using System.Text;
using NetworkUtil;

public class LogoutReqPacket : Packet
{
    public override EPacketType PacketType => EPacketType.LOGOUT_REQ;

    public uint UID { get; set; }

    public LogoutReqPacket() { }

    public LogoutReqPacket(uint uid)
    {
        UID = uid;
    }

    public override void Serialize(BinaryWriter writer)
    {
        writer.WriteUInt32(UID);
    }



    public override void Deserialize(BinaryReader reader)
    {
        UID = reader.ReadUInt32();
    }

    public override bool IsValid()
    {
        return UID != 0;
    }
}
