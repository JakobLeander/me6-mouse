/*
  Control the ME6 Mini Mouse over bluetooth
  Hardware
  - Arduino Nano
  - 3A UBEC (output 5v)
  - 9g servo
  - MX1508 Motor driver
  - Geared dual axis 6v Motor
  - HC-06 bluetooth module
  - DFPlayer Mini MP3 player
  - 2S Battery

  By using this sketch you can control the ME6 Mini Mouse over bluetooth
  After connecting to device you can send commands over serial

  The commands are in the format a character followed by a single digit:

  The commands must be wrapped by () to reduce risk of corrupted commands
  - '(D#)' = Drive 1 second, 1 = max backward, 5 = stop, 9 = max forward
  - '(T#)' = Turn, e.g. R0 = straight, R9 = turn maximum right
  - '(S#)' = Play sound, value does not matter

  You could also add a checksum to the command to further reduce risk of corrupted
  messages as serial communication over bluetooth is not 100% reliable

  To keep car driving continously you need to send a drive command about every half second
  This is my preferred approach to ensure the robot will stop moving if it looses connection

  2025-05-13 Jakob Leander
*/

#include <SoftwareSerial.h>
#include <Servo.h>

// Define the pins on the Arduino
#define RX_PIN 2        // Pin that is connected to HC-06 TX pin
#define TX_PIN 3        // Pin that is connected to HC-06 RX pin
#define IN1_PIN 5       // Pin connected to the motor driver IN1
#define IN2_PIN 6       // Pin connected to the motor driver IN1
#define SERVO_PIN 9     // Pin on control signal to servo
#define DFPLAYER_PIN 10 // Pin that is connected to the DFPlayer IO_2 port

// Bluetooth commands
const char ACTION_NOTHING = 'N'; // Drive command
const char ACTION_DRIVE = 'D';  // Drive command
const char ACTION_TURN = 'T';   // Turn command
const char ACTION_SOUND = 'S';  // Sound command

// Global variables
SoftwareSerial g_bluetoothSerial(RX_PIN, TX_PIN);
Servo g_servo;
unsigned long g_lastDriveRequest; // Last time a drive request was received

// Constants
const int MIN_MOTOR_SPEED = 120; // Does not work if too low
const int MAX_MOTOR_SPEED = 230;
const int MIN_SERVO_ANGLE = 60;                // Full left
const int MAX_SERVO_ANGLE = 120;               // Full right
const unsigned long MOTOR_RUN_TIME = 1000;     // Number of miliseconds to run the motor on each request
const unsigned long DFPLAYER_PRESS_TIME = 500; // Number of miliseconds to ground the pin for playing sound

// Setup ME6
void setup()
{
  Serial.begin(9600);
  g_bluetoothSerial.begin(9600);
  g_lastDriveRequest = millis();
  g_servo.attach(SERVO_PIN, 1000, 2000); // Define servo range
  Drive(5); // Stop motors
  Turn(5); // Center steering
  digitalWrite(DFPLAYER_PIN, LOW);
  delay(1000); // Keep the pin low for a while to increase volume
  pinMode(DFPLAYER_PIN, INPUT);
  delay(DFPLAYER_PRESS_TIME);
  PlaySound();
  Serial.println("ME6 is ready");
  g_bluetoothSerial.write("ME6 is ready");

}

// Main loop
void loop()
{
  byte action = ACTION_NOTHING;
  byte actionValue = 0;

  // If no request to move for a while, stop moving
  if ((millis() - g_lastDriveRequest) > MOTOR_RUN_TIME)
  {
    Drive(5); // Stop motors
  }

  // See if there are any new bluetooth commands
  ReadBluetoothCommand(action, actionValue);

  // If no action
  if (action == ACTION_NOTHING)
  {
    return;
  }

  // Handle actions
  if (action == ACTION_DRIVE)
  {
    Drive(actionValue);
    g_lastDriveRequest = millis();
  }

  if (action == ACTION_TURN)
  {
    Turn(actionValue);
  }

  if (action == ACTION_SOUND)
  {
    PlaySound();
  }
}

/*
 * Function:  Read bluetooth command
 * --------------------
 * Read bluetooth command from the HC-06 one character at a time
 * Identify a comand starting with '{' and ending with '}'
 *
 * I have included this so you can see how to do advanced serial communication
 * with a robot
 * Static variables are preserved between calls
 */
void ReadBluetoothCommand(byte &action, byte &actionValue)
{
  const char startMarker = '(';
  const char endMarker = ')';
  const byte bufferLength = 2;
  char c;
  static byte index = 0;
  static boolean recieveInProgress = false;
  static char buffer[bufferLength];
  
  action = ACTION_NOTHING;
  actionValue = 0;
  
  if (g_bluetoothSerial.available())
  {
    c = g_bluetoothSerial.read();

    // If we are starting a new command
    if (c == startMarker)
    {
      recieveInProgress = true;
      index = 0;
      return;
    }

    if (true == recieveInProgress)
    {
      // If we recieve end marker and we have recieved expected number of bytes
      if (c == endMarker && index == bufferLength)
      {
        recieveInProgress = false;
        index = 0;
        action = buffer[0];
        actionValue = buffer[1] - '0';
      }
      else
      {
        if (index >= bufferLength)
        {
          index = bufferLength - 1;
        }
        buffer[index] = c;
        index++;
      }
    }
  }
}

/*
 * Function:  Drive
 * --------------------
 * Drive forward, backward or stop
 * Speed is between 1 is full back, 9 full forward, 5 is stop
 */
void Drive(byte speed)
{
  int motorSpeed = 0;

  // Should we stop the motors?
  if (speed == 5)
  {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    return;
  }

  // Should we drive forward?
  if (speed > 5)
  {
    motorSpeed = map(speed, 6, 9, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
    Serial.println("Forward: " + String(motorSpeed));
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, motorSpeed);
    return;
  }

  // Should we drive backward?
  if (speed < 5)
  {
    motorSpeed = map(speed, 1, 4, MAX_MOTOR_SPEED, MIN_MOTOR_SPEED);
    Serial.println("Backward: " + String(motorSpeed));
    digitalWrite(IN2_PIN, LOW);
    analogWrite(IN1_PIN, motorSpeed);
    return;
  }
}

/*
 * Function:  Turn
 * --------------------
 * Turn steering
 * Direction is between 1 and 9, 1 full left, 5 straight, 9 full right
 */
void Turn(byte direction)
{
  Serial.println("direction: " + String(direction));
  direction = constrain(direction, 1, 9);
  int servoAngle = map(direction, 1, 9, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
  g_servo.write(servoAngle);
  Serial.println("Turn to servo angle: " + String(servoAngle));
}

/*
 * Function:  Play Sound
 * --------------------
 * Play next sound on the DFPlayer
 */
void PlaySound()
{
  pinMode(DFPLAYER_PIN, OUTPUT);
  digitalWrite(DFPLAYER_PIN, LOW);
  delay(DFPLAYER_PRESS_TIME);
  pinMode(DFPLAYER_PIN, INPUT);
  Serial.println("Play Sound");
}