#include <Arduino.h>
#include "LiquidCrystal.h"
#include "Dht11.h"


const uint8_t LCD_RS = 2;
const uint8_t LCD_READ_WRITE = 3;
const uint8_t LCD_ENABLE = 4;
LiquidCrystal lcd(LCD_RS, LCD_READ_WRITE, LCD_ENABLE, 5, 6, 7, 8, 9, 10, 11, 12);

const uint8_t DHT11_DATA_PIN = 13;
Dht11 dht11(DHT11_DATA_PIN);

const uint8_t TMP36_SENSOR = A0;

float Tmp36Temperature() {
  int arduino_value = analogRead(TMP36_SENSOR);
  float voltage_mv = arduino_value * 5.0 / 1023.0 * 1000.0;
  return (voltage_mv - 500) / 10;
}

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

// Setup the out pins
void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  lcd.begin(16, 2);
  Serial.print("DHT11 LIBRARY VERSION: ");
  Serial.println(Dht11::VERSION);
}

void loop() {
  Serial.println("Loop");

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

  Serial.print("Humidity (%):");
  Serial.println((float) dht11.getHumidity(), 2);

  Serial.print("Temperature (°C):");
  Serial.println((float) dht11.getTemperature(), 2);

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  // lcd.print(dht11.getTemperature());
  // lcd.print(" | ");
  lcd.print(Tmp36AvgTemperature(), 2);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(dht11.getHumidity());
  lcd.print('%');
  delay(500);
}
