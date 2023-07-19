#include <string.h>

int M0 = 1;
int M1 = 1;
int AUX = 1;
String id = "001";
String channel = "";
String address = "";
String frequency = "";
bool received_ACK = false;
// bool received__setup_string = false;
// bool done_setup = false;  // Flag: Done set up parameter for device
// bool R_SetUp = false;     //Flag: Env received message "SetUpParam" or not
String received__string = "";
int counter = 1;

String messageToEnv = "";
enum MODE_TYPE {
  MODE_0_NORMAL = 0,
  MODE_1_WAKE_UP = 1,
  MODE_2_POWER_SAVING = 2,
  MODE_3_SLEEP = 3,
  MODE_INIT = 3,

};

MODE_TYPE Mode;
bool ack = false;
bool setup_done = false;
byte buffer_read__user[512];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Mode = MODE_INIT;
}
String encodeMessage(String id, String channel, String address, String data) {
  String message = data;
  //Combine params to message
  return message;
}
void decodeMessage(String message) {
  char* token = strtok(message.c_str(), " ");
  String mess[6];
  int index = 0;
  while (token != NULL) {
    mess[index] = String(token);
    mess[index].trim();
    index++;
    token = strtok(NULL, " ");
  }
}
void serialEvent() {
  if (Serial.available()) {
    char receivedChar = Serial.read();   // Đọc một byte dữ liệu từ cổng nối tiếp
    if (receivedChar != '\n') {          // Kiểm tra nếu chưa gặp ký tự kết thúc chuỗi
      received__string += receivedChar;  // Thêm ký tự vào bộ đệm
    } else {
      // Ký tự kết thúc chuỗi được nhận
      // Xử lý bộ đệm (ví dụ: in ra dữ liệu)
      // Reset bộ đệm
      ack = true;
      counter = 1;
      received__string.trim();
      // processData(received__string);
      received__string = "";
    }
  }
}

// void processData(String message) {
//   if (Mode == MODE_3_SLEEP) {
//     if (ack) {
//       SetUpParameter();
//     }
//   }
//   ack = false;
// }

void SetUpParameter(String message) {
  // Serial.println("processing");
  decodeMessage(message);
  setup_done = true;
  ack = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (M0 == 0 && M1 == 0) {
    Mode = MODE_0_NORMAL;
  } else if (M0 == 0 && M1 == 1) {
    Mode = MODE_1_WAKE_UP;
  } else if (M0 == 1 && M1 == 0) {
    Mode = MODE_2_POWER_SAVING;
  } else {
    Mode = MODE_3_SLEEP;
  }

  switch (Mode) {
    case MODE_3_SLEEP:
      {
        if (!setup_done) {
          if (!ack) {
            counter--;
            if (counter <= 0) {
              counter = 1000;
              messageToEnv = encodeMessage(id, address, channel, "SetUpParam");
              Serial.println(messageToEnv);
            }
          } else {
            //set up param
            SetUpParameter();
          }

        } else {
          if (!ack) {
            counter--;
            if (counter <= 0) {
              counter = 1000;
              messageToEnv = encodeMessage(id, address, channel, "Success");
              Serial.println(messageToEnv);
            }
          }
        }
        break;
      }
    default:
      break;
  }
  delay(1);
}
