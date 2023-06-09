#include <Arduino.h>
#include <Servo.h>

#define DEGREE_MIN 45
#define DEGREE_MID 45
#define DEGREE_MAX 135
#define DEGREE_STEP 5

#define PIN_SERVO 9
#define PIN_EYE 8
#define PIN_GND 6

#define BRIGHT 197

Servo servo;

uint8_t degree = 0;
bool dir = true;

void blink(int times, bool isOn) {
  for (int i = 0; i < times; i++) {
    analogWrite(PIN_EYE, BRIGHT);
    delay(300);
    analogWrite(PIN_EYE, 0);
    delay(300);
  }
  analogWrite(PIN_EYE, isOn ? BRIGHT : 0);
}

void setup() {

  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);

  pinMode(PIN_EYE, OUTPUT);

  servo = Servo_attach(PIN_SERVO);

  blink(3, true);

  degree = DEGREE_MID;
  Servo_write(servo, degree);
  delay(1000 * 3);
}

void loop() {
  if (dir) {
    degree += DEGREE_STEP;
  } else {
    degree -= DEGREE_STEP;
  }
  if (degree > DEGREE_MAX) {
    dir = false;
    degree = DEGREE_MAX;
  } else if (degree < DEGREE_MIN) {
    dir = true;
    degree = DEGREE_MIN;
  }
  Servo_write(servo, degree);
  if (degree == DEGREE_MID || degree == DEGREE_MAX) {
    blink(3, true);
  }
  delay(1000 * 1);
}
