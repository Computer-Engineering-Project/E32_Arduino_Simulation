#include "Arduino.h"
#include "ReadMessage.h"

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
  String message = id + ":" + address + ":" + channel + ":" + data + ":" +e32_parameter.parity +":"+e32_parameter.uartRate+":"+e32_parameter.airRate+":"+e32_parameter.fixedMode+":"+e32_parameter.IOMode+":"+e32_parameter.wakeupTime+":"+e32_parameter.FEC+":"+e32_parameter.transPower;
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
    e32_parameter.address = mess[1] + mess[2];
    String sped = hexToBinary(mess[3]);
    String bin_parity = sped.substring(0, 2);
    bin_parity.trim();
    if (bin_parity == "00") {
      e32_parameter.parity = "8N1";
    } else if (bin_parity == "01") {
      e32_parameter.parity = "8O1";
    } else if (bin_parity == "10") {
      e32_parameter.parity = "8E1";
    } else {
      e32_parameter.parity = "8N1";
    }
    String bin_uart_rate = sped.substring(2, 5);
    bin_uart_rate.trim();
    if (bin_uart_rate == "000") {
      e32_parameter.uartRate = "1200";
    } else if (bin_uart_rate == "001") {
      e32_parameter.uartRate = "2400";
    } else if (bin_uart_rate == "010") {
      e32_parameter.uartRate = "4800";
    } else if (bin_uart_rate == "011") {
      e32_parameter.uartRate = "9600";
    } else if (bin_uart_rate == "100") {
      e32_parameter.uartRate = "19200";
    } else if (bin_uart_rate == "101") {
      e32_parameter.uartRate = "38400";
    } else if (bin_uart_rate == "110") {
      e32_parameter.uartRate = "57600";
    } else {
      e32_parameter.uartRate = "115200";
    }
    String bin_air_rate = sped.substring(5, 8);
    bin_air_rate.trim();
    if (bin_air_rate == "000") {
      e32_parameter.airRate = "0.3";
    } else if (bin_air_rate == "001") {
      e32_parameter.airRate = "1.2";
    } else if (bin_air_rate == "010") {
      e32_parameter.airRate = "2.4";
    } else if (bin_air_rate == "011") {
      e32_parameter.airRate = "4.8";
    } else if (bin_air_rate == "100") {
      e32_parameter.airRate = "9.6";
    } else if (bin_air_rate == "101") {
      e32_parameter.airRate = "19.2";
    } else {
      e32_parameter.airRate = "19.2";
    }
    e32_parameter.channel = mess[4];
    String option = hexToBinary(mess[5]);
    String bin_fixed_mode = option.substring(0, 1);
    if (bin_fixed_mode == "1") {
      e32_parameter.fixedMode = "Fixed";
    } else {
      e32_parameter.fixedMode = "Trans";
    }
    e32_parameter.IOMode = option.substring(1, 2);
    String bin_wake_time = option.substring(2, 5);
    if (bin_wake_time == "000") {
      e32_parameter.wakeupTime = "250";
    } else if (bin_wake_time == "001") {
      e32_parameter.wakeupTime = "500";
    } else if (bin_wake_time == "010") {
      e32_parameter.wakeupTime = "750";
    } else if (bin_wake_time == "011") {
      e32_parameter.wakeupTime = "1000";
    } else if (bin_wake_time == "100") {
      e32_parameter.wakeupTime = "1250";
    } else if (bin_wake_time == "101") {
      e32_parameter.wakeupTime = "1500";
    } else if (bin_wake_time == "110") {
      e32_parameter.wakeupTime = "1750";
    } else {
      e32_parameter.wakeupTime = "2000";
    }
    e32_parameter.FEC = option.substring(5, 6);
    String bin_power = option.substring(6);
    if (bin_power == "00") {
      e32_parameter.transPower = "20";
    } else if (bin_power == "01") {
      e32_parameter.transPower = "17";
    } else if (bin_power == "10") {
      e32_parameter.transPower = "14";
    } else {
      e32_parameter.transPower = "10";
    }
  }
}

