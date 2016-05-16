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

  if (result != Dht11::OK) {
      Serial.print("Error: ");
      Serial.println(result);
      return;
  }

  Serial.print("Humidity (%):");
  Serial.println((float) device.getHumidity(), 2);

  Serial.print("Temperature (°C):");
  Serial.println((float) device.getTemperature(), 2);
}
