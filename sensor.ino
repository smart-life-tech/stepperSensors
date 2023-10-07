#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>

#define STEPPER_PIN_STEP 5 // Step pin for stepper motor
#define STEPPER_PIN_DIR 4  // Direction pin for stepper motor
#define SENSOR1_PIN 2      // Sensor 1 input pin
#define SENSOR2_PIN 3      // Sensor 2 input pin
#define NEOPIXEL_PIN 6     // Neopixel data pin
#define BUZZER_PIN 9       // Buzzer pin

#define NUMPIXELS 15   // Number of Neopixels
#define MAX_SPEED 1000 // Maximum speed for the stepper motor

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
AccelStepper stepper(AccelStepper::DRIVER, STEPPER_PIN_STEP, STEPPER_PIN_DIR);
bool rotate = true;
void setup()
{
  pinMode(SENSOR1_PIN, INPUT_PULLUP);
  pinMode(SENSOR2_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  strip.begin();
  strip.show();

  stepper.setMaxSpeed(MAX_SPEED);
}

void loop()
{
  int sensor1State = digitalRead(SENSOR1_PIN);
  int sensor2State = digitalRead(SENSOR2_PIN);

  if (sensor1State == LOW && sensor2State == LOW)
  {
    if (rotate)
    {
      // Both sensors detect part
      stepper.stop();
      playSuccessSound();
      lightUpNeopixels();
      // delay(1500);
      // lightOffNeopixels();
      rotate = false;
    }
  }
  else
  {
    // Sensors don't detect part
    rotate = true;
    lightOffNeopixels();
    stepper.setSpeed(400);
    stepper.runSpeed();
  }
}

void playSuccessSound()
{
  // Implement sound logic for success
  tone(BUZZER_PIN, 200, 500); // Play a 1-second tone
}

void lightUpNeopixels()
{
  // Implement Neopixel lighting logic
  // Example:
  for (int i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red color for each Neopixel
  }
  strip.show();
}
void lightOffNeopixels()
{
  // Implement Neopixel lighting logic
  // Example:
  for (int i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Red color for each Neopixel
  }
  strip.show();
}