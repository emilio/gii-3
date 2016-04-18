// Ultrasonic Ranging Module HC - SR04
//
// https://www.sparkfun.com/products/9065
//
// https://www.arduino.cc/en/Tutorial/Sweep

#include <Servo.h>

Servo servo;
const int SERVO_OUT = 9;
const int MAX_ROTATION = 160;

void setup() {
  Serial.begin (9600);
  servo.attach(SERVO_OUT);
}

void loop() {
  for (int i = 0; i < MAX_ROTATION; ++i) {
    servo.write(i);
    delay(5);
  }

  for (int i = 0; i < MAX_ROTATION; ++i) {
    servo.write(MAX_ROTATION - i);
    delay(5);
  }
}
