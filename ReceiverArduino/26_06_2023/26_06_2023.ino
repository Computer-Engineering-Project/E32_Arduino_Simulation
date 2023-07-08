
#include "stdlib.h"
#include "inital.h"
#include "ReadMessage.h"


String readString;
String response_data;
String dataReceived;
bool stringCompleted = false;
bool dataSetUpSended = false;

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
      // struct Packet packet = DecodeStringToPacket(readString);
      // dataReceived = packet.data;
      // response_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, packet.owner_address, packet.owner_channel, "Received");
      response_data = "Received";
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


  delay(1);
  // }

  // Chờ 1 giây trước khi lặp lại
}