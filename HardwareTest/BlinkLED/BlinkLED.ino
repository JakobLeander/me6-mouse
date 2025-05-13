/*
  Blink the onboard LED on the Arduino Nano
  Use a distinct pattern
  2025-05-12 Jakob Leander
*/  
void setup() {
  // Initialize the LED Port
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite (LED_BUILTIN, HIGH) ;
  delay(250);
  digitalWrite (LED_BUILTIN, LOW) ;
  delay(250);
  digitalWrite (LED_BUILTIN, HIGH) ;
  delay(250);
  digitalWrite (LED_BUILTIN, LOW) ;
  delay(1000);
}
