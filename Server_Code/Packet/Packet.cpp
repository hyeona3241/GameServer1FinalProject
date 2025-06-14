#include "Packet.h"

Packet::Packet(PacketHeader header) : header(header)
{
}

Packet::Packet(EPacketType type, uint16_t size) : header(type, size)
{
}

