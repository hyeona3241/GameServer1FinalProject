using System;
using NetworkUtil;

/// <summary>
/// �α��� ������ Ȯ�� ��û ��Ŷ (Ŭ���̾�Ʈ �� ����)
/// �߰� ������ ���� ��Ŷ ����� ���Ե�
/// </summary>
public class LoginFinalizeReqPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_FINALIZE_REQ;

    public LoginFinalizeReqPacket() : base("") { }

    public override void Serialize(BinaryWriter writer)
    {
        // ������ ����: ��� �ܿ� ���� ���� ����
        // �� Serialize�� BinaryWriter.WriteHeader ���Ŀ� ȣ��ȴٰ� ����
    }

    public override void Deserialize(BinaryReader reader)
    {
        // �ƹ� ������ ����
    }

    public override bool IsValid() => true;
}
