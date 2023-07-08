#ifndef INITAL_H
#define INITAL_H

#include "Arduino.h"


struct Parameter
{
    int frequency; // Hz
    int baud_rate;
    String address;
    String channel;
    float air_data_rate; // kbps
    int parity;
    int transmitting_power;
};

struct Pin
{
    int RXD;
    int TXD;
    int M0;
    int M1;
    int AUX;
};

struct Packet
{
    char type;
    String owner_channel;
    String owner_address;
    String target_address;
    String target_channel;
    String data;
};

struct Parameter createParameter(int frequency, int baud_rate, String address, String channel, float air_data_rate, int parity, int transmitting_power);
struct Pin createPin(int RXD, int TXD, int M0, int M1, int AUX);
void inital();
String sendSetUpInfomation();

// Parameters--------------------------------------------------------
extern struct Parameter e32_parameter;
extern struct Pin e32_pin;

#endif
