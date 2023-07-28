#ifndef INITAL_H
#define INITAL_H

#include "Arduino.h"


struct Parameter {
  String id;
  String channel;
  String address;
  String frequency;
  String parity;
  String uartRate;
  String airRate;
  String fixedMode;
  String IOMode;
  String wakeupTime;
  String FEC;
  String transPower;
};

struct Pin {
  int RXD;
  int TXD;
  int M0;
  int M1;
  int AUX;
};
enum MODE_TYPE {
  MODE_0_NORMAL = 0,
  MODE_1_WAKE_UP = 1,
  MODE_2_POWER_SAVING = 2,
  MODE_3_SLEEP = 3,
  MODE_INIT = 3,

};

struct Parameter createParameter(String id, String channel, String address, String frequency, String parity, String uartRate, String airRate, String fixedMode, String IOMode, String wakeupTime, String FEC, String transPower);
struct Pin createPin(int RXD, int TXD, int M0, int M1, int AUX);
void inital();

// Parameters--------------------------------------------------------
extern struct Parameter e32_parameter;
extern struct Pin e32_pin;

#endif
