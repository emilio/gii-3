// LCD screen
//
// http://chipmanuals.com/datasheets/search/go/?query=LCD-1602GD1622y&nohistory=1&h=1
// http://www.engineersgarage.com/electronic-components/16x2-lcd-module-datasheet

const int READ_WRITE = 8;
const int ENABLE = 9;
const int RS = 10;

#include <LiquidCristal.h>

LiquidCrystal lcd(RS, ENABLE, 4, 5, 6, 7)

// Setup the out pins
void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello, world!");
}

void loop() {
  // TODO
}
