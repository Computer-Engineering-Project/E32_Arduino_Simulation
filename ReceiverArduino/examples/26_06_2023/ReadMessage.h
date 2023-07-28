#ifndef READ_MESSAGE_H
#define READ_MESSAGE_H

#include "Arduino.h"
#include "inital.h"

// Function prototypes
String encodeMessage(String id, String address, String channel, String data);
void decodeMessage(String message);

#endif
