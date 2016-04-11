// Ultrasonic Ranging Module HC - SR04
//
// Datasheet: http://www.micropik.com/PDF/HCSR04.pdf

const int trigPin = 13;
const int echoPin = 12;
const int led1 = 8;
const int led2 = 9;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

const float REF_MAX = 20.0;
const float REF_MIN = 10.0;

void loop() {
  unsigned long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2.0) / 29.1;

  // First read
  if (first < 0)
    first = distance;

  if (distance > REF_MAX) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  } else if (distance < REF_MIN) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }


  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  } else {
    Serial.print(first);
    Serial.print(", ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}
