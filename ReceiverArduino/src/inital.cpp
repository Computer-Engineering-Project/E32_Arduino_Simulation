#include "Arduino.h"
#include "inital.h"

struct Parameter createParameter(int frequency, int baud_rate, String address, String channel, float air_data_rate, int parity, int transmitting_power)
{
    struct Parameter parameter;
    parameter.address = address;
    parameter.frequency = frequency;
    parameter.baud_rate = baud_rate;
    parameter.channel = channel;
    parameter.parity = parity;
    parameter.transmitting_power = transmitting_power;

    return parameter;
}

struct Pin createPin(int RXD, int TXD, int M0, int M1, int AUX)
{
    struct Pin pin;
    pin.AUX = AUX;
    pin.M0 = M0;
    pin.M1 = M1;
    pin.RXD = RXD;
    pin.TXD = TXD;

    return pin;
}
void inital()
{
    e32_parameter = createParameter(433, 9600, "0010", "16", 2.4, 8, 100);
    e32_pin = createPin(0, 1, 2, 3, 4);
}
String sendSetUpInfomation() {
  String data = "i:";
  // sprintf(data, "i:%04X:%d", e32_parameter.address, e32_parameter.channel);
  data = data + e32_parameter.address + ":" + e32_parameter.channel;
  return data;
}

struct Parameter e32_parameter;
struct Pin e32_pin;