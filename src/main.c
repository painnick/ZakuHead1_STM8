#include <Arduino.h>
#include <Servo.h>

#define ANGLE_MID 90

#define PIN_SERVO 9
#define PIN_EYE 8
#define PIN_GND 6

#define BRIGHT 197

Servo servo;

uint8_t angle = 0;  // 0~180. Center 90
uint8_t minAngle = 45;
uint8_t maxAngle = 135;

bool isRightDir = true;  // true Right, false Left
bool isDirChanged = true;
uint8_t angleStep = 1;

uint8_t probability[10] = {5, 7, 8, 9, 10, 13, 14, 15, 20, 30};

void calculateServoAngle() {
  isDirChanged = false;
  if (isRightDir) {
    angle += angleStep;
  } else {
    angle -= angleStep;
  }
  if (angle > maxAngle) {
    isRightDir = false;
    angle = maxAngle;
    isDirChanged = true;
  } else if (angle < minAngle) {
    isRightDir = true;
    angle = minAngle;
    isDirChanged = true;
  }
}

void blink(int times, bool isOn) {
  for (int i = 0; i < times; i++) {
    analogWrite(PIN_EYE, 63);
    delay(300);
    analogWrite(PIN_EYE, 127);
    delay(300);
    analogWrite(PIN_EYE, 195);
    delay(300);
    analogWrite(PIN_EYE, 255);
    delay(300);
    analogWrite(PIN_EYE, 195);
    delay(300);
    analogWrite(PIN_EYE, 127);
    delay(300);
    analogWrite(PIN_EYE, 31);
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

  angle = ANGLE_MID;
  Servo_write(servo, angle);
  delay(1000 * 3);
}

void loop() {
  if (random(50) == 0) {
    angle = ANGLE_MID;
    Servo_write(servo, angle);
    delay(1000);
    blink(5, true);
    delay(1000);
  }

  if (isDirChanged) {
    angleStep = probability[random(10)];
    if (isRightDir) {
      maxAngle = random_minmax(115, 135);
    } else {
      minAngle = random_minmax(45, 90);
    }
  }

  calculateServoAngle();

  Servo_write(servo, angle);

  if (isDirChanged) {
    delay(1000);
    blink(2, true);
    delay(1000);
  }

  delay(1000 * 1);
}
