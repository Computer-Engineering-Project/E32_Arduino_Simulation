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
String response_data;
String dataReceived;
bool stringCompleted = false;
bool dataSetUpSended = true;

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
  e32_parameter = createParameter(433, 9600, "0010", "16", 2.4, 8, 100);
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

// struct Packet DecodeStringToPacket(const char* encodeString) {
//   char params[6][100];
//   char sub_str[100];
//   struct Packet packet;
//   strcpy(sub_str, encodeString);
//   int colon_position = findCharacter(sub_str, ':');
//   strcpy(sub_str, sub_str + colon_position + 1);
//   for (int i = 0; i < 5; i++) {
//     int colon_position = findCharacter(sub_str, ':');
//     strncpy(params[i], sub_str, (size_t)colon_position);
//     params[i][colon_position] = '\0';
//     strcpy(sub_str, sub_str + colon_position + 1);
//   }
//   packet.type = params[0][0];
//   // sscanf(params[1], "%04X", &packet.owner_address);
//   packet.owner_address = String(params[1]);
//   packet.owner_channel = String(params[2]);
//   // sscanf(params[3], "%04X", &packet.target_address);
//   packet.target_address = String(params[3]);
//   packet.target_channel = String(params[4]);
//   packet.data = String(sub_str);
//   int length = packet.data.length();
//   packet.data.remove(length - 1);
//   return packet;
// }
struct Packet DecodeStringToPacket(String encodeString) {
  String delimiter = ":";
  int delimiterIndex;
  int lastIndex = 0;
  int i = 0;
  String params[7];
  struct Packet packet;

  while ((delimiterIndex = encodeString.indexOf(delimiter, lastIndex)) != -1) {
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
    } else {
      struct Packet packet = DecodeStringToPacket(readString);
      dataReceived = packet.data;
      response_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, packet.owner_address, packet.owner_channel, "Received");
      // response_data = packet.data;
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
      response_data = "";
    } else {
      if (stringCompleted) {
        Serial.println(response_data);
        stringCompleted = false;
        digitalWrite(LED_BUILTIN, dataReceived.toInt());
      }
    }
  }
  // serialEvent();

  // Serial.println(test_data);

  delay(1);
  // }

  // Chờ 1 giây trước khi lặp lại
}