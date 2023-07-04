struct Parameter {
  int frequency;  //Hz
  int baud_rate;
  int address;
  int channel;
  float air_data_rate;  //kbps
  int parity;
  int transmitting_power;
};

struct Pin {
  int RXD;
  int TXD;
  int M0;
  int M1;
  int AUX;
};
struct Parameter e32_parameter;
struct Pin e32_pin;

struct Parameter createParameter(int frequency, int baud_rate, int address, int channel, float air_data_rate, int parity, int transmitting_power) {
  struct Parameter parameter;
  parameter.address = address;
  parameter.frequency = frequency;
  parameter.baud_rate = baud_rate;
  parameter.channel = channel;
  parameter.parity = parity;
  parameter.transmitting_power = transmitting_power;

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
  e32_parameter = createParameter(433, 9600, 0x0000, 0x17, 2.4, 8, 100);
  e32_pin = createPin(0, 1, 2, 3, 4);
}

String sendSetUpInfomation() {
  String data = "i:";
  // sprintf(data, "i:%04X:%d", e32_parameter.address, e32_parameter.channel);
  char hex_param_address[5];
  sprintf(hex_param_address, "%04X", e32_parameter.address);
  data = data + String(hex_param_address) + ":" + String(e32_parameter.channel);
  return data;
}

void setup() {
  // put your setup code here, to run once:
  inital();
  pinMode(e32_pin.M0, INPUT);
  pinMode(e32_pin.M1, INPUT);
  pinMode(e32_pin.AUX, OUTPUT);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  String setUpData = sendSetUpInfomation();
  Serial.println(setUpData);
}
int counter1 = 10000;
void loop() {
  // put your main code here, to run repeatedly:
  counter1--;
  if (counter1 <= 0) {
    counter1 = 10000;
    Serial.println("d:b:0000:17:0023:10:0#");
  }
  delay(1);
}
