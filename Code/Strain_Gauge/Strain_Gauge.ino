/* Fourth set of data, 9600 baud rate, 65 gram mass
OFFSET: 4294958576
WEIGHT: 65
SCALE:  134.138458
use scale.set_offset(4294958576); and scale.set_scale(134.138458);
in the setup of your project */

/*CALIBRATION
===========
remove all weight from the loadcell
and press enter

Determine zero weight offset
OFFSET: 24073

place a weight on the loadcell
enter the weight in (whole) grams and press enter
WEIGHT: 2268
SCALE:  -2.161376

use scale.set_offset(24073); and scale.set_scale(-2.161376);
in the setup of your project */

// Seemingly in working order now, ready to combine with the other code

#include "HX711.h"

#define OFFSET_FACTOR 4294958576 // scale offset factor obtained from scale calibration code
#define SCALE_FACTOR -7*134.138458 // negative of scale SCALE factor obtained from scale calibration code to account for outputted weights being negative, and multiplied by seven because 
// the reading is off for whatever reason.  Will try to fix later, but this is a working solution for now

#define DAT 3
#define CLK 2

HX711 scale;
double weightInPounds;

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
  Serial.println("Reading: " + String(weightInPounds) + " lbs\n");
  delay(5000);
}