#include "HX711.h"

const int DOUT = 13;
const int CLK = 12;
const double CALIBRATION_FACTOR = -2.20555800618; // from HX711_esp32_config

HX711 scale;

void setup() {
  Serial.begin(115200);
  Serial.println("Load Cell Interfacing with ESP32 - DIY CHEAP PERFECT");
  scale.begin(DOUT, CLK);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare(); // reset scale
}

void loop() {
  Serial.print("Weight (lbs): "); 
  double weightInPounds = scale.get_units(10) / 453.6;
  Serial.println(weightInPounds);
  scale.power_down(); // set ADC to sleep mode
  delay(1000);
  scale.power_up();
}