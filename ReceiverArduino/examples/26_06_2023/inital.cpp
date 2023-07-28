#include "Arduino.h"
#include "inital.h"

struct Parameter createParameter(String id, String channel, String address, String frequency, String parity, String uartRate, String airRate, String fixedMode, String IOMode, String wakeupTime, String FEC, String transPower) {
  struct Parameter parameter;
  parameter.id = id;
  parameter.address = address;
  parameter.frequency = frequency;
  parameter.uartRate = uartRate;
  parameter.channel = channel;
  parameter.parity = parity;
  parameter.airRate = airRate;
  parameter.IOMode = IOMode;
  parameter.wakeupTime = wakeupTime;
  parameter.FEC = FEC;
  parameter.transPower = transPower;

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
  e32_parameter = createParameter("001", "17", "0000", "433", "", "", "", "", "", "", "", "");
  e32_pin = createPin(0, 1, 2, 3, 4);
}

struct Parameter e32_parameter;
struct Pin e32_pin;