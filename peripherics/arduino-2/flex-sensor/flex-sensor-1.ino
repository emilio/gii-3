// Spectra Symbol 2.2" Sensor
//
// https://www.sparkfun.com/products/10264
//
// Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/FLEX%20SENSOR%20DATA%20SHEET%202014.pdf

const int SENSOR = A0;

// Arduino gives us the voltage between 0 and 5v,
// being it a number between 0 and 1023.
float voltage(int sensor) {
  int arduino_value = analogRead(sensor);
  return arduino_value * 5.0 / 1023.0 * 1000.0;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("read: ");
  Serial.println(voltage(SENSOR));

  delay(100);
}


