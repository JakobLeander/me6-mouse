/*
  Test that the servo can rotate
  https://docs.arduino.cc/libraries/servo/#Usage/Examples
  2025-05-12 Jakob Leander
*/
#include <Servo.h>

// PWM pin that is connected to the servo
#define SERVO_PIN 9

Servo myServo;       // create a servo object
int servoAngle = 90; // servo angle which can vary from 0 - 180

void setup()
{
  myServo.attach(SERVO_PIN, 1000, 2000); // Define servo range
}

void loop()
{
  myServo.write(120); // move servo to 30 deg
  delay(1500);
  myServo.write(60); // move servo to -30 deg
  delay(1500);
}
