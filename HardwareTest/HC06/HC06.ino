/*
  Test HC-06 Bluetooth Module
  https://www.martyncurrey.com/arduino-and-hc-06-zs-040/
  You can use this free android app to test with:
    https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en
  Make sure to use a voltage divider so RX pin of the HC-06 only gets 3.3 volt
  Connect both TX and RX pins of the HC-06 to the Arduino or it will not be stable
  2025-05-12 Jakob Leander
*/

const long sendPauseTime = 5000; // Time between sending data
#define RX_PIN 2                 // Pin that is connected to HC-06 TX pin
#define TX_PIN 3                 // Pin that is connected to HC-06 RX pin

#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(RX_PIN, TX_PIN);

char c = ' ';
unsigned long lastTimeStamp = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Arduino is ready");

  // HC-06 default serial speed for communication mode is 9600
  bluetoothSerial.begin(9600);
}

void loop()
{
  if ((millis() - lastTimeStamp) > sendPauseTime)
  {
    lastTimeStamp = millis();
    bluetoothSerial.write(" * Test * ");
  }

  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (bluetoothSerial.available())
  {
    c = bluetoothSerial.read();
    Serial.write(c);
  }
}
