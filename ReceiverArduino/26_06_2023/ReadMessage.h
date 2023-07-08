#ifndef READ_MESSAGE_H
#define READ_MESSAGE_H

#include "Arduino.h"
#include "inital.h"

// Function prototypes
String EncodePacketToString(char type, String owner_address, String owner_channel, String target_address, String target_channel, String data);
struct Packet DecodeStringToPacket(String encodeString);

#endif
