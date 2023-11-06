#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "pitches.h"

// Pin Definitions
#define RELAY_PIN 2
#define STEP_PIN 5
#define DIR_PIN 6
#define BUZZER_PIN 7
#define NEOPIXEL_PIN 8
#define TEMP_SENS 14
#define SENSOR1_PIN 3

// Constants
#define NUM_PIXELS 8
#define DELAY_INTERVAL 80

// NeoPixel Initialization
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Melody Definitions
int start_melody[] = {NOTE_E6, NOTE_E6, NOTE_G6, NOTE_C7};
int start_noteDurations[] = {2, 12, 12, 4};

// OneWire and DallasTemperature Initialization
OneWire oneWire(TEMP_SENS);
DallasTemperature tempSensor(&oneWire);

bool rotate = true;
int sensor1State = 1;
// Motor Variables
AccelStepper stepper(AccelStepper::FULL4WIRE, STEP_PIN, DIR_PIN);
void myISR() {
  // This is the Interrupt Service Routine (ISR)
  // It will be called when a change is detected on pin 3
  // Put your code here to respond to the interrupt event
  if (digitalRead(SENSOR1_PIN)) {
    sensor1State = 0;
  } else {
    sensor1State = 1;
  }
  Serial.println("Interrupt detected on pin 3!");
}

// Function to play startup melody
void playStartupMelody() {
  for (int i = 0; i < 4; i++) {
    int noteDuration = 1000 / start_noteDurations[i];
    tone(BUZZER_PIN, start_melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(BUZZER_PIN);
  }
}

void rotateMotor(int steps, bool direction, int speed) {
  digitalWrite(DIR_PIN, direction);

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speed);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SENSOR1_PIN, INPUT_PULLUP);
  pinMode(TEMP_SENS, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Attach an interrupt to pin 3 (interrupt 1)
  attachInterrupt(digitalPinToInterrupt(SENSOR1_PIN), myISR, CHANGE);
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize the NeoPixel strip to "off" state
  digitalWrite(RELAY_PIN, LOW); // Fan is initially turned off

  playStartupMelody();

  //Serial.begin(9600);
  tempSensor.begin();

  // Set motor speed and acceleration
  stepper.setMaxSpeed(1000); // Set your desired motor speed
  stepper.setAcceleration(500); // Set your desired acceleration
}


void loop() {

  // int sensor1State = digitalRead(SENSOR1_PIN);


  if (sensor1State == LOW) {

    if (rotate) {

      // Part detected

      strip.fill(strip.Color(255, 0, 0), 0, 2);

      strip.setPixelColor(6, strip.Color(0, 255, 0)); // Green

      strip.setPixelColor(7, strip.Color(0, 255, 0)); // Green

      strip.show();

      stepper.stop(); // Stop the motor

      rotate = false;

    }

  }

  else {

    // No part detected

    strip.fill(strip.Color(0, 255, 0), 0, 2);

    strip.setPixelColor(6, strip.Color(255, 0, 0)); // Red

    strip.setPixelColor(7, strip.Color(255, 0, 0)); // Red

    strip.show();

    rotateMotor(100, 1, 250); // Steps  ;  Direction (0 & 1)  ;  Speed (0-Slow, 1000-Fast)

    rotate = true;

  }


  tempSensor.requestTemperatures();

  float currentTemp = tempSensor.getTempCByIndex(0);

  //Serial.println(currentTemp);

  // Check if the temperature is above 28 degrees Celsius

  if (currentTemp > 25) {

    digitalWrite(RELAY_PIN, HIGH); // Turn on the fan

  } else {

    digitalWrite(RELAY_PIN, LOW); // Turn off the fan

  }

}