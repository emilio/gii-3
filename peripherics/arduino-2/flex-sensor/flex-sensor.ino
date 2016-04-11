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

const int LED_MAX = 12;
const int LED_MIN = 13;

void setup() {
  Serial.begin(9600);
  pinMode(LED_MIN, OUTPUT);
  pinMode(LED_MAX, OUTPUT);
}

float clamp(float value, float min, float max) {
  return min(max(value, min), max);
}

const float VOLTAGE_MIN = 300.0;
const float VOLTAGE_MAX = 1500.0;

void loop() {
  Serial.print("read: ");
  float v = clamp(voltage(SENSOR), VOLTAGE_MIN, VOLTAGE_MAX);

  Serial.println(v);

  // Arduino gives us a value between 300mV and 1500mV approx.
  // So we'll just take a value between 0 and 1 and turn on the
  // led that better suits our needs.
  float weight = (v - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN);
  if (weight > .5) {
    digitalWrite(LED_MIN, LOW);
    digitalWrite(LED_MAX, HIGH);
  } else {
    digitalWrite(LED_MAX, LOW);
    digitalWrite(LED_MIN, HIGH);
  }

  delay(100);
}
