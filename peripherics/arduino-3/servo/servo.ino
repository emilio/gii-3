// Simple Servomotor
//
// https://www.sparkfun.com/products/9065
//
// https://www.arduino.cc/en/Tutorial/Sweep

#include <Servo.h>

Servo servo;
const int SERVO_OUT = 9;
// Technically it supports up to 160
// but we're constrained by the exercise statement
const int MAX_ROTATION = 90;
const int LIGHT_IN = A1;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_OUT);
  pinMode(LIGHT_IN, INPUT);
}

int MIN = -1;
int MAX = -1;
void loop() {
  // Normally we get values in the range 25 - 1000
  int light = analogRead(LIGHT_IN);
  Serial.println(light);

  if (MIN == -1 || light < MIN) {
    MIN = light;
  }

  if (MAX == -1 || light > MAX) {
    MAX = light;
  }

  int angle = map(light, MIN, MAX, 0, 160);

  servo.write(angle);
  delay(5);
}
