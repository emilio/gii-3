void setup() {
  pinMode(13, OUTPUT);
}

int current = 0;

void loop() {
  analogWrite(13, current);
  delay(200);

  current += 10;
  current = current % 256;
}
