#include "HX711.h"

// NOTE: The SG should has an EV of 5 V rather than 9 V as is expected from the datasheet, and the readings seem to be accurate +- 2 pounds roughly.
// This was configured this way because of oversight regarding the Sparkfun Hx711 amplifier that expects a 5 V supply to the SG.  If redesigned,
// either a different amplifier breakout board or different SG should be chosen.  For this use case, it should be fine however because our readings
// are on the order of hundreds of pounds, and the forces causing resistive changes on the SG should be plenty high to be amplified and read into the 
// ADC even though the supply voltage of the SG is only 5 V rather than the desired 9-12 V.

#define OFFSET_FACTOR 4294958576 
#define SCALE_FACTOR -7*134.138458 

#define DAT 3
#define CLK 2

HX711 scale;
double weightInPounds;

void setup() {
  Serial.begin(9600);
  scale.begin(DAT,CLK);
  scale.set_offset(OFFSET_FACTOR);
  scale.set_scale(SCALE_FACTOR);
  scale.tare(); // Assuming there is no weight on the scale at start up.
}

void loop() {
  if (scale.is_ready()) {
    weightInPounds = scale.get_units(10); // average reading out of 10 measurements
    Serial.println("Reading: " + String(weightInPounds) + " lbs\n");
  }
  else {
    Serial.println("Scale wasn't ready to read.");
  }
  delay(5000);
}