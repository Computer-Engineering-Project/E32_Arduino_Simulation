#include "stdlib.h"
#include "inital.h"
#include "ReadMessage.h"



String readString;
String response_data = "test_data";
bool stringCompleted = false;
bool dataSetUpSended = false;
bool ledOn = false;


void serialEvent() {
  if (Serial.available() > 0) {
    readString = Serial.readString();
    delay(1);
    readString.trim();
    //makes the string readString
  }
  if (readString.length() > 0) {
    if (readString == "SuccessSetUp") {
      response_data = "";
      dataSetUpSended = true;
    } else {
      struct Packet packet = DecodeStringToPacket(readString.c_str());
      if(packet.data == "Received") {
        ledOn = !ledOn;
      }
    }
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
    } else {
      if (!ledOn) {
        Serial.println("d:b:0000:17:0010:16:0#");
      } else {
        Serial.println("d:b:0000:17:0010:16:1#");
      }
    }
  }
  // serialEvent();

  // Serial.println(test_data);

  delay(1);
  // }

  // Chờ 1 giây trước khi lặp lại
}