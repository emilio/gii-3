// http://www.micro4you.com/files/sensor/DHT11.pdf
// http://playground.arduino.cc/Main/DHT11Lib

#include <Arduino.h>
#include "Dht11.h"

const uint8_t DHT11_DATA_PIN = 2;

Dht11 device(DHT11_DATA_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(Dht11::VERSION);
  Serial.println();
}

void loop() {
  int result = device.read();

  switch (result) {
  case Dht11::OK:
    break;
  case Dht11::ERROR_TIMEOUT:
    Serial.println("Timeout");
    break; // Keep going
  case Dht11::ERROR_CHECKSUM:
    Serial.println("Checksum error");
    return;
  default:
    Serial.println("WTF?");
    return;
  }

  Serial.print("Humidity (%):");
  Serial.println((float) device.getHumidity(), 2);

  Serial.print("Temperature (°C):");
  Serial.println((float) device.getTemperature(), 2);
}
