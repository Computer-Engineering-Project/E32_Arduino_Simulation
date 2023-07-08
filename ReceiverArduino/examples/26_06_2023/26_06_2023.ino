
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
      readString = Serial.readString();
      delay(10);
      readString.trim();
      //makes the string readString
    }
  }

  if (readString.length() > 0) {
    if (readString == "SuccessSetUp") {
      response_data = "";
      dataSetUpSended = true;
    }
    // else if (readString == "Received") {
    //   dataSendSuccess = true;
    // }
    else {
      struct Packet packet = DecodeStringToPacket(readString.c_str());
      response_data = EncodePacketToString('b', e32_parameter.address, e32_parameter.channel, packet.owner_address, packet.owner_channel, packet.data);
      if (packet.data == "1") {
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (packet.data == "0") {
        digitalWrite(LED_BUILTIN, LOW);
      }
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
  if (!dataSetUpSended) {
    counter--;
    if (counter <= 0) {
      counter = 3000;
      response_data = sendSetUpInfomation();
      Serial.println(response_data);
      response_data = "";
    }

  } else {
    if (response_data != "") {
      Serial.println(response_data);
      response_data = "";
      readString = "";
    }
  }


  delay(1);
  // }

  // Chờ 1 giây trước khi lặp lại
}