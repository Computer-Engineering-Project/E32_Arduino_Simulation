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
  char data[];
};

struct Parameter e32_parameter;
struct Pin e32_pin;
struct Packet trans_packet;

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

const char* EncodePacketToString(char type, int owner_address, int owner_channel, int target_address, int target_channel, char* data) {
  char* value = (char*)malloc((strlen(data) + 100) * sizeof(char));
  sprintf(value, "d:%c:%04X:%d:%04X:%d:%s", type, owner_address, owner_channel, target_address, target_channel, data);
  value[strlen(value)] = '\0';
  return value;
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
  strcpy(packet.data, sub_str);
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
  const char* test_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, 25, 17, "test");

  // Send sensor value to Python
  Serial.println(test_data);
  // Chờ 1 giây trước khi lặp lại
  delay(1000);
}