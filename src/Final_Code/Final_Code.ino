// NOTE: The SG has an EV of 5 V rather than 9 V as is expected from the datasheet, and the readings seem to be accurate +- 2 pounds roughly.
// This was configured this way because of oversight regarding the Sparkfun Hx711 amplifier that expects a 5 V supply to the SG.  If redesigned,
// either a different amplifier breakout board or different SG should be chosen.  In our use case, it is acceptable because the readings are on the order
// of hundreds of pounds, meaning not having a super fine grain resolution won't make a substantial difference in our readings.

#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include "HX711.h"

#define ANM_SIGNAL_PIN 6
#define CAL_ZERO_SPEED_ADC_READING 80
#define MAX_SPEED_ADC_READING 409
#define MAX_WIND_SPEED 32.4 // in m/s
#define MPH_CONVERSION_FACTOR 2.23694
#define CLK 2
#define DAT 3
#define MAX485_DE_RE 8
#define MS_RX 10
#define MS_TX 11
#define WV_SLAVE_ADDRESS 2
#define WV_DEGREE_REGISTER_ADDRESS 0x0000
#define OFFSET_FACTOR 4294958576 
#define SCALE_FACTOR -7*134.138458 // working calibration factor, don't change!
#define BAUD_RATE 9600
#define DELAY_MSEC 5000


ModbusMaster node;
HX711 scale;

SoftwareSerial mySerial(MS_RX,MS_TX); 

double weight = 0;
double curr_wind_speed = 0;
int anm_adc_signal_voltage = 0;

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
  // csv output format will be time, wind direction (degrees, 0 is due north), wind speed (mph), weight (lbs)
  weight = scale.get_units(10); // average 10 readings for weight output
  anm_adc_signal_voltage = analogRead(ANM_SIGNAL_PIN); 
  curr_wind_speed = (MAX_WIND_SPEED/(MAX_SPEED_ADC_READING-CAL_ZERO_SPEED_ADC_READING))*(anm_adc_signal_voltage-CAL_ZERO_SPEED_ADC_READING);
  curr_wind_speed *= MPH_CONVERSION_FACTOR; // converting from m/s to mph
  uint8_t wv_register_read_result;
  wv_register_read_result = node.readHoldingRegisters(0x0000, 1); 
  if (wv_register_read_result == node.ku8MBSuccess)
  {
    Serial.println(String(node.getResponseBuffer(0x0)/10.0f) + "," + String(weight) + "," + String(curr_wind_speed)); 
  } else {
    // If the RS485 communication failed, write a -1 to where the wind vane reading would be.
    Serial.println("-1," + String(weight) + "," + String(curr_wind_speed));
  }
  // Will want to make this delay as long as possible, will ask Doug how often we want to be reading data.
  delay(DELAY_MSEC);
}