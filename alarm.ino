#include <NewTone.h>
#include <NewPing.h>

#define LED_PIN 8
#define BUTTON_PIN 7
#define ECHO_PIN 5
#define TRIGGER_PIN 4
#define MAX_DISTANCE 50
#define TONE_PIN 12

bool state = false;
int buttonState;
int lastButtonState = HIGH;

const int frequency = 2500;

// Distance units are on centimeters 
int distance = 450;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long pingTimer = 0;
unsigned long previousMillis = 0;
unsigned int pingSpeed = 50;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pingTimer = millis();
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  
  if (millis() >= pingTimer) {
    pingTimer += pingSpeed;
    sonar.ping_timer(echoCheck);
  }
  
  unsigned long currentMillis = millis();
  
  if ((currentMillis - previousMillis) >= distance * 10 && distance < MAX_DISTANCE) {
    previousMillis = currentMillis;
    state = !state;
  }

  if (state) {
    NewTone(TONE_PIN, frequency);
    digitalWrite(LED_PIN, HIGH);
  } else {
    noNewTone(TONE_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        state = false;
        distance = 450;
      }
    }
  }

  lastButtonState = reading;
}

void echoCheck() {
  if (sonar.check_timer()) {
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
  }
}
