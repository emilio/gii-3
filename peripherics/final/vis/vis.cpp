// LCD screen
//
// http://chipmanuals.com/datasheets/search/go/?query=LCD-1602GD1622y&nohistory=1&h=1
// http://www.engineersgarage.com/electronic-components/16x2-lcd-module-datasheet

#include <Arduino.h>

const uint8_t READ_WRITE = 9;
const uint8_t ENABLE = 10;
const uint8_t RS = 8;

#include "LiquidCrystal.h"

LiquidCrystal lcd(RS, READ_WRITE, ENABLE,
                  11, 12, 2, 3, 4, 5, 6, 7);

// Setup the out pins
void setup() {
  lcd.begin(16, 2);
  Serial.begin(115200);
  Serial.println("Setup");
  lcd.print("Hello, world!");
}

void loop() {
  Serial.println("Loop");
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  delay(1000);
}
