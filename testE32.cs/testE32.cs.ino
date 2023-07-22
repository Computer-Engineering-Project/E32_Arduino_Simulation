int pin_M0 = 2;
int pin_M1 = 3;
int pin_AUX = 4;

int M0 = 1;
int M1 = 0;
int AUX = 1;
String id = "002";
String channel = "";
String address = "";
String frequency = "";
String parity = "";
String uartRate = "";
String airRate = "";
String fixedMode = "Fixed";
String IOMode = "";
String wakeupTime = "250";
String FEC = "";
String transPower = "";
String targetAddress = "";
String targetChannel = "";
String dataRaw = "";
bool received_ACK = false;
// bool received__setup_string = false;
// bool done_setup = false;  // Flag: Done set up parameter for device
// bool R_SetUp = false;     //Flag: Env received message "SetUpParam" or not
String Received__Env = "";
String Received__User = "";
int counter = 1;
int counterMode2 = (int)wakeupTime.toInt();

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
char buffer__user[512];
char buffer__env[512];
int bufferIndex_user = 0;
int bufferIndex_env = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Mode = MODE_INIT;
  pinMode(pin_M0, INPUT);
  pinMode(pin_M1, INPUT);
  pinMode(pin_AUX, OUTPUT);
}

uint8_t hexCharToDecimal(char hexChar) {
  if (hexChar >= '0' && hexChar <= '9') {
    return hexChar - '0';
  } else if (hexChar >= 'A' && hexChar <= 'F') {
    return hexChar - 'A' + 10;
  } else if (hexChar >= 'a' && hexChar <= 'f') {
    return hexChar - 'a' + 10;
  } else {
    return 0;
  }
}
String hexToBinary(String hexString) {
  String binaryString = "";

  for (int i = 0; i < hexString.length(); i++) {
    char hexChar = hexString.charAt(i);
    uint8_t decimalValue = hexCharToDecimal(hexChar);

    for (int j = 3; j >= 0; j--) {
      char bitValue = (decimalValue >> j) & 1 ? '1' : '0';
      binaryString += bitValue;
    }
  }

  return binaryString;
}
String encodeMessage(String id, String address, String channel, String data) {
  String message = id + ":" + address + ":" + channel + ":" + data;
  // + ":" +parity +":"+uartRate+":"+airRate+":"+fixedMode+":"+IOMode+":"+wakeupTime+":"+FEC+":"+transPower;
  //Combine params to message
  return message;
}
void decodeMessage(String message) {
  if (message != "") {
    char* token = strtok(message.c_str(), " ");
    String mess[6];
    int index = 0;
    while (token != NULL) {
      mess[index] = String(token);
      mess[index].trim();
      index++;
      token = strtok(NULL, " ");
    }
    for (int i = 0; i < 6; i++) {
      mess[i].trim();
    }
    address += mess[1] + mess[2];
    String sped = hexToBinary(mess[3]);
    String bin_parity = sped.substring(0, 2);
    bin_parity.trim();
    if (bin_parity == "00") {
      parity = "8N1";
    } else if (bin_parity == "01") {
      parity = "8O1";
    } else if (bin_parity == "10") {
      parity = "8E1";
    } else {
      parity = "8N1";
    }
    String bin_uart_rate = sped.substring(2, 5);
    bin_uart_rate.trim();
    if (bin_uart_rate == "000") {
      uartRate = "1200";
    } else if (bin_uart_rate == "001") {
      uartRate = "2400";
    } else if (bin_uart_rate == "010") {
      uartRate = "4800";
    } else if (bin_uart_rate == "011") {
      uartRate = "9600";
    } else if (bin_uart_rate == "100") {
      uartRate = "19200";
    } else if (bin_uart_rate == "101") {
      uartRate = "38400";
    } else if (bin_uart_rate == "110") {
      uartRate = "57600";
    } else {
      uartRate = "115200";
    }
    String bin_air_rate = sped.substring(5, 8);
    bin_air_rate.trim();
    if (bin_air_rate == "000") {
      airRate = "0.3";
    } else if (bin_air_rate == "001") {
      airRate = "1.2";
    } else if (bin_air_rate == "010") {
      airRate = "2.4";
    } else if (bin_air_rate == "011") {
      airRate = "4.8";
    } else if (bin_air_rate == "100") {
      airRate = "9.6";
    } else if (bin_air_rate == "101") {
      airRate = "19.2";
    } else {
      airRate = "19.2";
    }
    channel = mess[4];
    String option = hexToBinary(mess[5]);
    String bin_fixed_mode = option.substring(0, 1);
    if (bin_fixed_mode == "1") {
      fixedMode = "Fixed";
    } else {
      fixedMode = "Trans";
    }
    IOMode = option.substring(1, 2);
    String bin_wake_time = option.substring(2, 5);
    if (bin_wake_time == "000") {
      wakeupTime = "250";
    } else if (bin_wake_time == "001") {
      wakeupTime = "500";
    } else if (bin_wake_time == "010") {
      wakeupTime = "750";
    } else if (bin_wake_time == "011") {
      wakeupTime = "1000";
    } else if (bin_wake_time == "100") {
      wakeupTime = "1250";
    } else if (bin_wake_time == "101") {
      wakeupTime = "1500";
    } else if (bin_wake_time == "110") {
      wakeupTime = "1750";
    } else {
      wakeupTime = "2000";
    }
    FEC = option.substring(5, 6);
    String bin_power = option.substring(6);
    if (bin_power == "00") {
      transPower = "20";
    } else if (bin_power == "01") {
      transPower = "17";
    } else if (bin_power == "10") {
      transPower = "14";
    } else {
      transPower = "10";
    }
  }
}

void serialEvent1() {
  if (Serial1.available()) {
    char receivedChar = Serial1.read();  // Đọc một byte dữ liệu từ cổng nối tiếp
    if (receivedChar != '\n') {          // Kiểm tra nếu chưa gặp ký tự kết thúc chuỗi
      Received__User += receivedChar;    // Thêm ký tự vào bộ đệm
    } else {
      // Ký tự kết thúc chuỗi được nhận
      // Xử lý bộ đệm (ví dụ: in ra dữ liệu)
      // Reset bộ đệm
      // Serial.println(Received__User);
      Received__User.trim();
      processData__User(Received__User);
      // received__string = "";
    }
  }
}

void serialEvent() {
  if (Serial.available()) {
    char receivedChar = Serial.read();  // Đọc một byte dữ liệu từ cổng nối tiếp
    if (receivedChar != '\n') {         // Kiểm tra nếu chưa gặp ký tự kết thúc chuỗi
      Received__Env += receivedChar;    // Thêm ký tự vào bộ đệm
    } else {
      // Ký tự kết thúc chuỗi được nhận
      // Xử lý bộ đệm (ví dụ: in ra dữ liệu)
      // Reset bộ đệm
      Received__Env.trim();
      processData__Env(Received__Env);
      
      ack = true;
      counter = 1;
      // processData(received__string);

      // received__string = "";
    }
  }
}


void processData__User(String receivedString) {
  // bufferIndex = 0;
  int length = receivedString.length();
  if (length > 0) {
    for (int i = 0; i < length; i++) {
      char data = receivedString.charAt(i);
      buffer__user[bufferIndex_user] = data;
      bufferIndex_user++;
    }
  }
  Received__User = "";
}

void processData__Env(String receivedString) {
  int length = receivedString.length();
  if (length > 0) {
    for (int i = 0; i < length; i++) {
      char data = receivedString.charAt(i);
      buffer__env[bufferIndex_env] = data;
      bufferIndex_env++;
    }
  }
  Received__Env = "";
}

void SetUpParameter(String message) {
  // Serial.println("processing");
  decodeMessage(message);
  setup_done = true;
  ack = false;
}

String addPreamble_Code(String dataRaw) {
  int time = (int)wakeupTime.toInt();
  int index = (int)time / 250;
  String preambleCode = "";
  for (int i = 0; i < index; i++) {
    preambleCode += "W";
  }
  if (index > 0) {
    String result = preambleCode + "#" + dataRaw;
    return result;
  }
  return dataRaw;
}
String removePreamble_Code(String message) {
  char* token = strtok(message.c_str(), "#");
  String mess[2];
  int index = 0;
  while (token != NULL) {
    mess[index] = String(token);
    mess[index].trim();
    index++;
    token = strtok(NULL, "#");
  }
  for (int i = 0; i < 2; i++) {
    mess[i].trim();
  }
  return mess[1];
}


void loop() {
  // put your main code here, to run repeatedly:
  M0 = digitalRead(pin_M0);
  M1 = digitalRead(pin_M1);

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
              messageToEnv = "";
            }
          } else {
            //set up param
            SetUpParameter(Received__Env);
          }

        } else {
          if (!ack) {
            counter--;
            if (counter <= 0) {
              counter = 1000;
              messageToEnv = encodeMessage(id, address, channel, "Success");
              Serial.println(messageToEnv);
              messageToEnv = "";
            }
          }
        }
        break;
      }
    case MODE_0_NORMAL:
      {
        if (bufferIndex_user > 0 && AUX == 1) {
          AUX = 0;
          String message = "";
          for (int i = 0; i < bufferIndex_user; i++) {
            message += buffer__user[i];
          }
          message.trim();
          // Serial.println("Message assigned from buffer:" + message + ":"+bufferIndex);
          if (fixedMode == "Fixed") {
            char* token = strtok(message.c_str(), " ");
            String mess[4];
            int index = 0;
            while (token != NULL) {
              mess[index] = String(token);
              mess[index].trim();
              index++;
              token = strtok(NULL, " ");
            }
            for (int i = 0; i < 4; i++) {
              mess[i].trim();
            }
            targetAddress = mess[0] + mess[1];
            targetChannel = mess[2];
            dataRaw = mess[3];
          } else {
            dataRaw = message;
          }
          bufferIndex_user = 0;
        }

        if (dataRaw.length() > 0) {
          if (fixedMode == "Fixed") {
            messageToEnv = encodeMessage(id, targetAddress, targetChannel, dataRaw);
          } else {
            messageToEnv = encodeMessage(id, "", "", dataRaw);
          }
          Serial.println(messageToEnv);
          messageToEnv = "";
          dataRaw = "";
          AUX = 1;
        }
        if (AUX == 1 && bufferIndex_env > 0) {
          AUX = 0;
          String message = "";
          for (int i = 0; i < bufferIndex_env; i++) {
            message += buffer__env[i];
          }
          message.trim();
          if (message.indexOf('#') != -1) {
            message = removePreamble_Code(message);
          }
          Serial1.println(message);
          bufferIndex_env = 0;
          AUX = 1;
        }
        break;
      }
    case MODE_1_WAKE_UP:
      {
        if (bufferIndex_user > 0 && AUX == 1) {
          AUX = 0;
          String message = "";
          for (int i = 0; i < bufferIndex_user; i++) {
            message += buffer__user[i];
          }
          message.trim();
          // Serial.println("Message assigned from buffer:" + message + ":"+bufferIndex);
          if (fixedMode == "Fixed") {
            char* token = strtok(message.c_str(), " ");
            String mess[4];
            int index = 0;
            while (token != NULL) {
              mess[index] = String(token);
              mess[index].trim();
              index++;
              token = strtok(NULL, " ");
            }
            for (int i = 0; i < 4; i++) {
              mess[i].trim();
            }
            targetAddress = mess[0] + mess[1];
            targetChannel = mess[2];
            dataRaw = mess[3];
          } else {
            dataRaw = message;
          }
          bufferIndex_user = 0;
        }

        if (dataRaw.length() > 0) {
          dataRaw = addPreamble_Code(dataRaw);
          if (fixedMode == "Fixed") {
            messageToEnv = encodeMessage(id, targetAddress, targetChannel, dataRaw);
          } else {
            messageToEnv = encodeMessage(id, "", "", dataRaw);
          }
          Serial.println(messageToEnv);
          messageToEnv = "";
          dataRaw = "";
          AUX = 1;
        }
        if (AUX == 1 && bufferIndex_env > 0) {
          AUX = 0;
          String message = "";
          for (int i = 0; i < bufferIndex_env; i++) {
            message += buffer__env[i];
          }
          message.trim();
          if (message.indexOf('#') != -1) {
            message = removePreamble_Code(message);
          }
          Serial1.println(message);
          bufferIndex_env = 0;
          AUX = 1;
        }
        break;
      }
    case MODE_2_POWER_SAVING:
      {
        counterMode2--;
        if (counterMode2 <= 0) {

          counterMode2 = (int)wakeupTime.toInt();
          if (AUX == 1 && bufferIndex_env > 0) {
            AUX = 0;
            String message = "";
            for (int i = 0; i < bufferIndex_env; i++) {
              message += buffer__env[i];
            }
            message.trim();

            if (message.indexOf('W') == -1) {
              bufferIndex_env = 0;
              AUX = 1;
              break;
            }

            message = removePreamble_Code(message);
            Serial1.println(message);
            bufferIndex_env = 0;
            AUX = 1;
          }
        }
        break;
      }
    default:
      break;
  }
  delay(1);
}
