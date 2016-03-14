// TMP 36 temperature sensor
//
// Look up: RTD.
//
// To work, they use thermistors[1] internally,
// but they manage to have a linear out/voltage relation.
//
// To work with this specific one:
// T(°C) = [(Vout in mV) - 500] / 10
//
// [1]: https://en.wikipedia.org/wiki/Thermistor
const int SENSOR = A0;

// Arduino gives us the voltage between 0 and 5v,
// being it a number between 0 and 1023.
float temperature() {
  int arduino_value = analogRead(SENSOR);
  float voltage_mv = arduino_value * 5.0 / 1023.0 * 1000.0;
  return (voltage_mv - 500) / 10;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  float current = temperature();
  Serial.print("temp (deg):");
  Serial.println(current);

  delay(100);
}


