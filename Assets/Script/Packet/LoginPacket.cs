using NetworkUtil;
using System.Text;

public abstract class LoginPacket : Packet
{
    public string Nickname { get; protected set; }

    protected LoginPacket(string nickname)
    {
        Nickname = nickname;
    }

    protected LoginPacket() { }

    public override bool IsValid()
    {
        return !string.IsNullOrEmpty(Nickname) && Encoding.UTF8.GetByteCount(Nickname) <= 255;
    }
}
