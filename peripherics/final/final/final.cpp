#include <Arduino.h>
#include "LiquidCrystal.h"
#include "Dht11.h"

// LCD screen's dimensions
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;
// LCD pins
const uint8_t LCD_RS = 2;
const uint8_t LCD_READ_WRITE = 3;
const uint8_t LCD_ENABLE = 4;
LiquidCrystal lcd(LCD_RS,
                  LCD_READ_WRITE,
                  LCD_ENABLE,
                  5, 6, 7, 8, 9, 10, 11, 12);

// Tresholds to show a warning
const float TEMPERATURE_MAX = 25.0;
const float TEMPERATURE_MIN = 18.0;

// DHT11 humidity sensor's data pin
const uint8_t DHT11_DATA_PIN = 13;
Dht11 dht11(DHT11_DATA_PIN);

// TMP 36 RTD sensor's pin.
const uint8_t TMP36_SENSOR = A0;

// This gives the TMP 36 RTD sensor's temperature in degrees,
// using the equation:
//
// T(°C) = [(Vout in mV) - 500] / 10
float Tmp36Temperature() {
  int arduino_value = analogRead(TMP36_SENSOR);
  float voltage_mv = arduino_value * 5.0 / 1023.0 * 1000.0;
  return (voltage_mv - 500) / 10;
}

// This uses the last four reads of the TMP 36 and
// returns the average to avoid huge variations.
float Tmp36AvgTemperature() {
    const size_t READ_COUNT = 4;
    static float last_reads[READ_COUNT] = {0};
    static size_t read_until = 0;
    static size_t cursor = 0;

    if (read_until < READ_COUNT) {
        last_reads[read_until] = Tmp36Temperature();
        read_until++;
        return last_reads[read_until - 1];
    }

    last_reads[cursor] = Tmp36Temperature();
    cursor = (cursor + 1) % READ_COUNT;
    float avg = 0;
    for (size_t i = 0; i < READ_COUNT; ++i)
        avg += last_reads[i];
    return avg / READ_COUNT;
}

void setup() {
  Serial.begin(115200);
  Serial.print("DHT11 LIBRARY VERSION: ");
  Serial.println(Dht11::VERSION);

  lcd.begin(LCD_COLS, LCD_ROWS);
}

void loop() {
  static bool show_warning = false;
  int result = dht11.read();

  switch (result) {
  case Dht11::OK:
    Serial.println("Read ok");
    break;
  case Dht11::ERROR_TIMEOUT:
    Serial.println("Timeout");
    return;
  case Dht11::ERROR_CHECKSUM:
    Serial.println("Checksum error");
    return;
  default:
    Serial.println("WTF?");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  // lcd.print(dht11.getTemperature());
  // lcd.print(" | ");
  // We trust more the thermistor than the humidity sensor...
  // The last one was giving **really** unrealistic results.
  float temperature = Tmp36AvgTemperature();
  lcd.print(temperature, 2);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(dht11.getHumidity());
  lcd.print('%');

  if (temperature > TEMPERATURE_MAX ||
          temperature < TEMPERATURE_MIN)
      show_warning = !show_warning;
  else
      show_warning = false;

  lcd.setCursor(LCD_COLS - 3, 0);
  if (show_warning)
      lcd.print("(!)");
  else
      lcd.print("   ");

  delay(500);
}
