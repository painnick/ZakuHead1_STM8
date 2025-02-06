#include <Arduino.h>
#include <Servo.h>

#define ANGLE_MID 90

#define PIN_SERVO 9
#define PIN_EYE 8
#define PIN_GND 6

#define BRIGHT 197

Servo servo;

int currentAngle = 90; // 0~180. Center 90
int minAngle = 45;
int maxAngle = 135;

bool isRightDir = true; // true Right, false Left
int blinkProb[7] = {0, 0, 0, 0, 2, 2, 3};

void blink(int times, bool isOn)
{
  for (int i = 0; i < times; i++)
  {
    analogWrite(PIN_EYE, 63);
    delay(250);
    analogWrite(PIN_EYE, 127);
    delay(250);
    analogWrite(PIN_EYE, 195);
    delay(250);
    analogWrite(PIN_EYE, 255);
    delay(250);
    analogWrite(PIN_EYE, 195);
    delay(250);
    analogWrite(PIN_EYE, 127);
    delay(250);
    analogWrite(PIN_EYE, 31);
    delay(250);
  }
  analogWrite(PIN_EYE, isOn ? BRIGHT : 0);
}

void moveServo(int targetAngle, int step)
{
  int lastAngle = currentAngle;

  if (currentAngle < targetAngle)
  { // 30 => 90
    targetAngle = min(targetAngle, maxAngle);
    for (int nextAngle = currentAngle; nextAngle < targetAngle; nextAngle += step)
    {
      nextAngle = min(nextAngle, maxAngle);
      Servo_write(servo, nextAngle);
      delay(100);
      lastAngle = nextAngle;
    }
  }
  else if (currentAngle > targetAngle)
  { // 90 => 30
    targetAngle = max(targetAngle, minAngle);
    for (int nextAngle = currentAngle; nextAngle > targetAngle; nextAngle -= step)
    {
      nextAngle = max(nextAngle, minAngle);
      Servo_write(servo, nextAngle);
      delay(100);
      lastAngle = nextAngle;
    }
  }
  currentAngle = lastAngle;
}

void setup()
{
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);

  pinMode(PIN_EYE, OUTPUT);
  servo = Servo_attach(PIN_SERVO);

  blink(2, true);

  currentAngle = ANGLE_MID;
  Servo_write(servo, currentAngle);
  delay(1000 * 3);
}

void loop()
{
  if (random(7) == 0 && abs(currentAngle - 90) > 15)
  {
    moveServo(ANGLE_MID, random_minmax(1, 5));
    delay(500);
    blink(random_minmax(2, 4), true);
    delay(1000);

    isRightDir = (random(10) % 2) == 0;
  }

  int blinkCount = blinkProb[random(6)];
  if (isRightDir)
  {
    moveServo(currentAngle + random_minmax(30, 100), random_minmax(1, 8));
  }
  else
  {
    moveServo(currentAngle - random_minmax(30, 100), random_minmax(1, 8));
  }

  if (blinkCount > 0)
  {
    delay(500);
    blink(blinkCount, true);
  }
  delay(1000);

  isRightDir = !isRightDir;
}
