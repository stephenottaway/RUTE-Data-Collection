const int speed_pin = A1;
const int delay_msec = 3000;

int analog_to_digital = 0;
double wind_speed = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  analog_to_digital = analogRead(speed_pin); 
  // min output of analogRead with the anemometer is ~85 (corresponding to ~0.4 V or ~0 m/s wind speed) up to ~433 (corresponding to ~2.0 V or ~32.4 m/s wind speed), based on the model we derived from measurements of voltage vs analog_output)
  // 0.4 volts is across signal and ground (for multimeter measurements, when wind speed is 0 m/s)
  wind_speed = (32.2/348)*(analog_to_digital-85);
  Serial.println("analog output: " + String(analog_to_digital) + " wind speed: " + String(wind_speed) + " m/s\n");
  delay(delay_msec);
}