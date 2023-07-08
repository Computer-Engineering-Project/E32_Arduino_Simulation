#include "Arduino.h"
#include "ReadMessage.h"

struct Packet DecodeStringToPacket(String encodeString)
{
    String delimiter = ":";
    int delimiterIndex;
    int lastIndex = 0;
    int i = 0;
    String params[7];
    struct Packet packet;

    while ((delimiterIndex = encodeString.indexOf(delimiter, lastIndex)) != -1)
    {
        String token = encodeString.substring(lastIndex, delimiterIndex);
        lastIndex = delimiterIndex + delimiter.length();

        // Serial.println(token);
        params[i] = token;
        i++;
    }

    // Print the remaining part after the last delimiter
    String lastToken = encodeString.substring(lastIndex);
    params[6] = lastToken;
    packet.type = params[1][0];
    packet.owner_address = params[2];
    packet.owner_channel = params[3];
    packet.target_address = params[4];
    packet.target_channel = params[5];
    packet.data = params[6];
    int length = packet.data.length();
    packet.data.remove(length - 1);
    return packet;
}
String EncodePacketToString(char type, String owner_address, String owner_channel, String target_address, String target_channel, String data)
{
    String result = "d:";

    result = result + type + ":" + owner_address + ":" + owner_channel + ":" + target_address + ":" + target_channel + ":" + data + "#";
    return result;
}
