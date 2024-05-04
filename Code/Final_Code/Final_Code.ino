#include "HX711.h"
#define SPEED_PIN 1
#define CLK 2
#define DAT 3
#define DELAY_MSEC 3000
#define OFFSET_FACTOR 4294958576 // scale offset factor obtained from scale calibration code
#define SCALE_FACTOR -7*134.138458 // negative of scale SCALE factor obtained from scale calibration code to account for outputted weights being negative, and multiplied by seven because 
// the reading is off for whatever reason.  Will try to fix later, but this is a working solution for now.

HX711 scale;

double weightInPounds = 0;
double wind_speed = 0;
int analog_to_digital = 0;

void setup() {
  Serial.begin(9600);
  scale.begin(DAT,CLK);
  scale.set_offset(OFFSET_FACTOR);
  scale.set_scale(SCALE_FACTOR);
  scale.tare(); // Assuming there is no weight on the scale at start up, reset the scale to 0. WILL LIKELY NEED TO GET RID OF LATER TO ACCOUNT FOR 
  // LATENT TENSION ON STRAIN GAUGE DURING THE SETUP
}

void loop() {
  weightInPounds = scale.get_units();
  analog_to_digital = analogRead(SPEED_PIN); 
  wind_speed = (32.2/348)*(analog_to_digital-85);
  wind_speed *= 2.23694; // converting from m/s to mph
  Serial.println("Reading: " + String(weightInPounds) + " lbs\n");
  delay(DELAY_MSEC);
  Serial.println("analog output: " + String(analog_to_digital) + " wind speed: " + String(wind_speed) + " mph\n");
  delay(DELAY_MSEC); 
}