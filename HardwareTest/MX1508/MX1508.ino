/*
  Run the motor with the MX1508 motor driver
  https://www.techmonkeybusiness.com/pdfs/MX1508_DC_Motor_Control.pdf
  2025-05-12 Jakob Leander
*/
const int IN1PIN = 5; // 0 to 255 (analogWrite)
const int IN2PIN = 6; // 0 to 255 (analogWrite)
const long activeTime = 2000;
const long pauseTime = 500;
const int slowSpeed = 120; // Does not work if too low
const int highSpeed = 200;
boolean forward = true;

void setup()
{
  pinMode(IN1PIN, OUTPUT);
  pinMode(IN2PIN, OUTPUT);
  digitalWrite(IN1PIN, LOW);
  digitalWrite(IN2PIN, LOW);
}

void loop()
{
  // Set the direction of the motor
  if (forward)
  {
    digitalWrite(IN1PIN, LOW);
    analogWrite(IN2PIN, slowSpeed);
    delay(activeTime);
    analogWrite(IN2PIN, highSpeed);
    delay(activeTime);
  }
  else
  {
    digitalWrite(IN2PIN, LOW);
    analogWrite(IN1PIN, slowSpeed);
    delay(activeTime);
    analogWrite(IN1PIN, highSpeed);
    delay(activeTime);
  }

  // Reverse direction
  forward = !forward;

  // Stop the motor
  digitalWrite(IN1PIN, LOW);
  digitalWrite(IN2PIN, LOW);
  delay(pauseTime);
}
