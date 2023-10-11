#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include "pitches.h"

#define STEPPER_PIN_STEP 6  // Step pin for stepper motor
#define STEPPER_PIN_DIR 5   // Direction pin for stepper motor
#define SENSOR1_PIN 2       // Sensor 1 input pin
#define SENSOR2_PIN 3      // Sensor 2 input pin
#define NEOPIXEL_PIN 13      // Neopixel data pin
#define BUZZER_PIN 12        // Buzzer pin

#define NUMPIXELS 8         // Number of LED
#define MAX_SPEED 1000      // Maximum speed for the stepper motor

// START MELODY ----------------------------------------------------------------
int start_melody[] = {
  NOTE_C7, NOTE_E7, NOTE_C8
};
int start_noteDurations[] = {
  12, 14, 8
};
// -----------------------------------------------------------------------------

// END MELODY ------------------------------------------------------------------
int stop_melody[] = {
  NOTE_C6, NOTE_E5, NOTE_C4
};
int stop_noteDurations[] = {
  12, 14, 8
};
// -----------------------------------------------------------------------------

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
AccelStepper stepper(AccelStepper::DRIVER, STEPPER_PIN_STEP, STEPPER_PIN_DIR);
bool rotate = false;

void setup() {
  pinMode(SENSOR1_PIN, INPUT_PULLUP);
  pinMode(SENSOR2_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  strip.begin();
  strip.setBrightness(10);

  start_tune();
  pixel_on_green();
}

void loop() {
  int sensor1State = digitalRead(SENSOR1_PIN);
  int sensor2State = digitalRead(SENSOR2_PIN);

  if (sensor1State == LOW && sensor2State == LOW) {
    if (rotate) {
      // Both sensors detect part
      strip.setPixelColor(0, 255, 0, 0);
      strip.setPixelColor(1, 255, 0, 0);
      strip.show();
      stepper.stop();
      //delay(1500);
      //lightOffNeopixels();
      rotate = false;
    }
  } else {
    // Sensors don't detect part
    rotate = true;
    strip.setPixelColor(0, 0, 255, 0);
    strip.setPixelColor(1, 0, 255, 0);
    strip.show();
    rotateMotor(150, 0, 800); // Steps  ;  Direction (0 & 1)  ;  Speed (0-Slow, 1000-Fast)
    delay(500);
  }
}

void rotateMotor(int  steps, bool direction, int speed){
  digitalWrite(STEPPER_PIN_DIR, direction);

  for (int i=0; i<steps; i++){
    digitalWrite(STEPPER_PIN_STEP, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEPPER_PIN_STEP, LOW);
    delayMicroseconds(speed);
  }
}

// -------------  BUZZER PROGRAM  -------------------------------------------------------------------------------------------------
void start_tune(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 800 / start_noteDurations[thisNote];
    tone(BUZZER_PIN, start_melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void stop_tune(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 3; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 800 / stop_noteDurations[thisNote];
    tone(BUZZER_PIN, stop_melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}
// -------------  END OF PROGRAM  ----------------------------------------------------------------------------------------------

void playSuccessSound() {
  // Implement sound logic for success
  tone(BUZZER_PIN, 200, 500);  // Play a 1-second tone
}

void lightUpNeopixels() {
  // Implement Neopixel lighting logic
  // Example:
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red color for each Neopixel
  }
  strip.show();
}
void lightOffNeopixels() {
  // Implement Neopixel lighting logic
  // Example:
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Red color for each Neopixel
  }
  strip.show();
}

// -------------  LED PROGRAM  -------------------------------------------------------------------------------------------------
void pixel_on_green() {
    strip.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called

  // turn pixels to green one by one with delay between each pixel
  for (int pixel = 0; pixel < NUMPIXELS; pixel++) {           // for each pixel
    strip.setPixelColor(pixel, strip.Color(0, 255, 0));  // it only takes effect if pixels.show() is called
    strip.show();                                           // send the updated pixel colors to the NeoPixel hardware.

    delay(100);  // pause between each pixel
  }
  delay(1000);
  // turn off all pixels for two seconds
  strip.clear();
  strip.show();  // send the updated pixel colors to the NeoPixel hardware.
}

void pixel_error_blink() {
    strip.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
    
  for (int pixel = 0; pixel < NUMPIXELS; pixel++) {           // for each pixel
    strip.setPixelColor(pixel, strip.Color(255, 0, 0));  // it only takes effect if pixels.show() is called
    strip.show();                                           // send the updated pixel colors to the NeoPixel hardware.

    delay(1);  // pause between each pixel
  }
  delay(3000);
  // turn off all pixels for two seconds
  strip.clear();
  strip.show();  // send the updated pixel colors to the NeoPixel hardware.
}
// -------------  END OF PROGRAM  ------------------------------------------------------------------------------------------------