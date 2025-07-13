const int signal_pin = A6;
const int delay_msec = 3000;
const int calibrated_zero_speed_adc_reading = 80;
const int max_speed_adc_reading = 409; // 2.0 V reading according to analogRead documentation, can't really calibrate this but won't make a huge difference in speed readings.
const double max_wind_speed = 32.4; // in m/s, corresponds to 2V across signal and ground or ~409 reading on ADC.

int adc_signal_voltage = 0;
double curr_wind_speed = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  adc_signal_voltage = analogRead(signal_pin); 
  curr_wind_speed = (max_wind_speed/(max_speed_adc_reading-calibrated_zero_speed_adc_reading))*(adc_signal_voltage-calibrated_zero_speed_adc_reading);
  Serial.println("ADC reading: " + String(adc_signal_voltage) + " wind speed: " + String(curr_wind_speed) + " m/s\n");
  delay(delay_msec);
}