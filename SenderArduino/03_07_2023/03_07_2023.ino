#include "string.h"
#include "stdlib.h"

struct Parameter {
  int frequency;  //Hz
  int baud_rate;
  String address;
  String channel;
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
  String owner_channel;
  String owner_address;
  String target_address;
  String target_channel;
  String data;
};

struct Parameter e32_parameter;
struct Pin e32_pin;
String readString;
String response_data = "test_data";
bool stringCompleted = false;
bool dataSetUpSended = false;
bool dataSendSuccess = false;

struct Parameter createParameter(int frequency, int baud_rate, String address, String channel, float air_data_rate, int parity, int transmitting_power) {
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
  e32_parameter = createParameter(433, 9600, "0000", "17", 2.4, 8, 100);
  e32_pin = createPin(0, 1, 2, 3, 4);
}

String EncodePacketToString(char type, String owner_address, String owner_channel, String target_address, String target_channel, String data) {
  String result = "d:";

  result = result + type + ":" + owner_address + ":" + owner_channel + ":" + target_address + ":" + target_channel + ":" + data + "#";
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
  // sscanf(params[1], "%04X", &packet.owner_address);
  packet.owner_address = String(params[1]);
  packet.owner_channel = String(params[2]);
  // sscanf(params[3], "%04X", &packet.target_address);
  packet.target_address = String(params[3]);
  packet.target_channel = String(params[4]);
  packet.data = String(sub_str);
  int length = packet.data.length();
  packet.data.remove(length - 1);
  return packet;
}

String sendSetUpInfomation() {
  String data = "i:";
  // sprintf(data, "i:%04X:%d", e32_parameter.address, e32_parameter.channel);
  data = data + e32_parameter.address + ":" + e32_parameter.channel;
  return data;
}

void serialEvent() {
  while (Serial.available()) {
    if (Serial.available() > 0) {
      char c = (char)Serial.read();  //gets one byte from serial buffer
      readString += c;
      if (c == '#') {
        stringCompleted = true;
      }  //makes the string readString
    }
  }
  if (readString.length() > 0) {
    if (readString == "s") {
      response_data = sendSetUpInfomation();
    } else if (readString == "SuccessSetUp") {
      response_data = "";
      dataSetUpSended = true;
    } else if (readString == "Received") {
      dataSendSuccess = true;
    } else {
      // struct Packet packet = DecodeStringToPacket(readString.c_str());
      // response_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, packet.owner_address, packet.owner_channel, packet.data);
      response_data = readString;
    }
  } else {
    response_data = "Can't read! Fail";
  }
}


void setup() {

  // put your setup code here, to run once:
  inital();
  pinMode(e32_pin.M0, INPUT);
  pinMode(e32_pin.M1, INPUT);
  pinMode(e32_pin.AUX, OUTPUT);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // char data[100];
  // sprintf(data, "i:%04X:%d", e32_parameter.address, e32_parameter.channel);
  // data[strlen(data)] = '\0';
  // Serial.println(data);
}
int counter = 10;
void loop() {
  // put your main code here, to run repeatedly:
  counter--;
  if (counter <= 0) {
    counter = 3000;
    if (!dataSetUpSended) {
      response_data = sendSetUpInfomation();
      Serial.println(response_data);
    } else {
      if (!dataSendSuccess) {
        Serial.println("d:b:0000:17:0010:16:0#");
      } else {
        Serial.println("d:b:0000:17:0010:16:1#");
        dataSendSuccess = false;
      }
    }
  }
  // serialEvent();

  // Serial.println(test_data);

  delay(1);
  // }

  // Chờ 1 giây trước khi lặp lại
}