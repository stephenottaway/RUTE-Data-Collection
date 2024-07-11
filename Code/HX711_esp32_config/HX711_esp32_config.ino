#include "HX711.h"

const int DOUT = 13;
const int CLK = 12;

HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(DOUT, CLK);
  scale.tare(); // reset scale
}

void loop() {
  if (scale.is_ready()) {
    Serial.println("HX711 reading: ");
    Serial.println(scale.get_value(5)); // Display 5 readings average after subtracting the tare weight from ADC
  } else {
      Serial.println("HX711 not found.");
      delay(500);
  }
}