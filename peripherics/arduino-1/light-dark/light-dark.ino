// Light-detector circuit and conversion between
// analog and digital signals
//
// Reference:  https://www.arduino.cc/en/Tutorial/PWM

// The input pin where the current passing by
// the ressistance is going to enter
const int ANALOG_INPUT = A0;

// The output pin
const int ANALOG_OUTPUT = 9;

void setup() {
  pinMode(ANALOG_OUTPUT, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int input_value = analogRead(ANALOG_INPUT);
  int digital = 0;
  // Experimentally we only received values between 300 and ~600.
  // If not this will be the correct code:
  // int digital = map(input_value, 0, 1023, 0, 255);
  if (input_value > 400)
    digital = map(input_value, 300, 650, 0, 255);

  Serial.print("in: ");
  Serial.println(input_value);

  analogWrite(ANALOG_OUTPUT, digital);
}
