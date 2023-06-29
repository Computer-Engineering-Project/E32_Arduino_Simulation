#include "string.h"
#include "stdlib.h"

struct Parameter {
  int frequency;  //Hz
  int baud_rate;
  int address;
  int channel;
  float air_data_rate;  //kbps
  int parity;
  int transmitting_power;
};

struct Pin {
  int RXD;
  int TXD;
  int M0;
  int M1;
  int AUX;
};

struct Packet {
  char type;
  int owner_channel;
  int owner_address;
  int target_address;
  int target_channel;
  String data;
};

struct Parameter e32_parameter;
struct Pin e32_pin;

struct Parameter createParameter(int frequency, int baud_rate, int address, int channel, float air_data_rate, int parity, int transmitting_power) {
  struct Parameter parameter;
  parameter.address = address;
  parameter.frequency = frequency;
  parameter.baud_rate = baud_rate;
  parameter.channel = channel;
  parameter.parity = parity;
  parameter.transmitting_power = transmitting_power;

  return parameter;
}

struct Pin createPin(int RXD, int TXD, int M0, int M1, int AUX) {
  struct Pin pin;
  pin.AUX = AUX;
  pin.M0 = M0;
  pin.M1 = M1;
  pin.RXD = RXD;
  pin.TXD = TXD;

  return pin;
}

void inital() {
  e32_parameter = createParameter(433, 9600, 0x0000, 0x17, 2.4, 8, 100);
  e32_pin = createPin(0, 1, 2, 3, 4);
}

String EncodePacketToString(char type, int owner_address, int owner_channel, int target_address, int target_channel, String data) {
  String result = "d:";
  char hex_owner_add[5];
  char hex_target_add[5];
  sprintf(hex_owner_add, "%04X", owner_address);
  sprintf(hex_target_add, "%04X", target_address);

  result = result + type + ":" + String(hex_owner_add) + ":" + String(owner_channel) + ":" + String(hex_target_add) + ":" + String(target_channel) + ":" + data;
  return result;
}

int findCharacter(const char* input, char target) {
  const char* position = strchr(input, target);

  if (position != NULL) {
    return position - input;  // Calculate the position by subtracting the base address
  } else {
    return -1;  // Character not found
  }
}

struct Packet DecodeStringToPacket(const char* encodeString) {
  char params[6][100];
  char sub_str[100];
  struct Packet packet;
  strcpy(sub_str, encodeString);
  int colon_position = findCharacter(sub_str, ':');
  strcpy(sub_str, sub_str + colon_position + 1);
  for (int i = 0; i < 5; i++) {
    int colon_position = findCharacter(sub_str, ':');
    strncpy(params[i], sub_str, (size_t)colon_position);
    params[i][colon_position] = '\0';
    strcpy(sub_str, sub_str + colon_position + 1);
  }
  packet.type = params[0][0];
  sscanf(params[1], "%04X", &packet.owner_address);
  packet.owner_channel = atoi(params[2]);
  sscanf(params[3], "%04X", &packet.target_address);
  packet.target_channel = atoi(params[4]);
  packet.data = String(sub_str);
  return packet;
}

void setup() {

  // put your setup code here, to run once:
  inital();
  pinMode(e32_pin.M0, INPUT);
  pinMode(e32_pin.M1, INPUT);
  pinMode(e32_pin.AUX, OUTPUT);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  char data[100];
  sprintf(data, "i:%04X:%d", e32_parameter.address, e32_parameter.channel);
  data[strlen(data)] = '\0';
  Serial.println(data);
}

void loop() {
  // put your main code here, to run repeatedly:
  String test_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, 25, 17, "test");

  // Send sensor value to Python

  // Serial.println(test_data);

  String readString;
  while (Serial.available()) {
    delay(50);
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c;         //makes the string readString
    }
  }
  if (readString.length() > 0) {
    struct Packet packet = DecodeStringToPacket(readString.c_str());
    String response_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, packet.owner_address, packet.owner_channel, packet.data);
    Serial.println(response_data);
  }
  delay(1000);
  // }

  // Chờ 1 giây trước khi lặp lại
}