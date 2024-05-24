#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include "HX711.h"

#define SPEED_PIN 1
#define CLK 2
#define DAT 3
#define MAX485_DE_RE 4
#define MS_RX 10
#define MS_TX 11
#define WV_SLAVE_ADDRESS 2
#define WD_DEGREE_REGISTER_ADDRESS 0x0000
#define OFFSET_FACTOR 4294958576 
#define SCALE_FACTOR -7*134.138458 // factor of -7 there to read correct weights, may need to be changed
#define BAUD_RATE 9600
#define DELAY_MSEC 3000


ModbusMaster node;
HX711 scale;

SoftwareSerial mySerial(MS_RX,MS_TX); 

double weight = 0;
double wind_speed = 0;
int analog_to_digital = 0;

void preTransmission()
{
  digitalWrite(MAX485_DE_RE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_DE_RE, 0);
}

void setup() 
{
  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, 0);
  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE);
  node.begin(WV_SLAVE_ADDRESS, mySerial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  scale.begin(DAT,CLK);
  scale.set_offset(OFFSET_FACTOR);
  scale.set_scale(SCALE_FACTOR);
  scale.tare(); // This line is only necessary if nothing is on the scale during setup
}

void loop() 
{
  // csv output format will be time, wind direction (degrees, 0 is due north), wind speed (mph), weight (lbs), weight (lbs) / wind force (mph) 
  weight = scale.get_units();
  analog_to_digital = analogRead(SPEED_PIN); 
  wind_speed = (32.2/348)*(analog_to_digital-85);
  wind_speed *= 2.23694; // converting from m/s to mph
  if (wind_speed == 0)
  {
    wind_speed += 0.01;
  }
  uint8_t result;
  result = node.readHoldingRegisters(WD_DEGREE_REGISTER_ADDRESS, 1);  // only reading 1 register address
  if (result == node.ku8MBSuccess)
  {
    Serial.println(String(node.getResponseBuffer(0x0)/10.0f) + "," + String(weight) + "," + String(wind_speed) + "," + String((double) weight/wind_speed)); 
  }
  delay(DELAY_MSEC);
}