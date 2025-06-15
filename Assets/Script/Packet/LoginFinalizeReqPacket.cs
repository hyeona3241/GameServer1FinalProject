using System;
using NetworkUtil;

/// <summary>
/// 로그인 마지막 확인 요청 패킷 (클라이언트 → 서버)
/// 추가 데이터 없이 패킷 헤더만 포함됨
/// </summary>
public class LoginFinalizeReqPacket : LoginPacket
{
    public override EPacketType PacketType => EPacketType.LOGIN_FINALIZE_REQ;

    public LoginFinalizeReqPacket() : base("") { }

    public override void Serialize(BinaryWriter writer)
    {
        // 데이터 없음: 헤더 외에 별도 본문 없음
        // 이 Serialize는 BinaryWriter.WriteHeader 이후에 호출된다고 가정
    }

    public override void Deserialize(BinaryReader reader)
    {
        // 아무 데이터 없음
    }

    public override bool IsValid() => true;
}
