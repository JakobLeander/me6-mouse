/*
  Test that the DFPlayer can play a sound
  https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
  To play next a sound briefly ground the IO_2 pin
  OBS: We use a trick of setting the DFPLAYER_PIN between sounds to INPUT to ensure it is neither high nor low
  You can adjust volume manually by grounding IO_1 (lower) or IO_2 (higher) for a longer period of time
  2025-05-12 Jakob Leander
*/

#define DFPLAYER_PIN 10      // Pin that is connected to the player IO_2 port
const long pressTime = 500;  // Time to ground the pin
const long waitTime = 5000; // Time to wait between sounds

void setup()
{
    digitalWrite(DFPLAYER_PIN, LOW);
    delay(waitTime); // Do a long grounding to increase volume
    pinMode(DFPLAYER_PIN, INPUT);
}

void loop()
{
    pinMode(DFPLAYER_PIN, OUTPUT);
    digitalWrite(DFPLAYER_PIN, LOW);
    delay(pressTime);

    // Wait for the sound to finish
    pinMode(DFPLAYER_PIN, INPUT);
    delay(waitTime);
}
