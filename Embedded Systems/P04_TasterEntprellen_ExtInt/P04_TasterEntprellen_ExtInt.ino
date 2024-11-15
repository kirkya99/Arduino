#include <Arduino.h>
#include <avr/interrupt.h>

void upKeyPressed();
void downKeyPressed();

const int UP_KEY_PIN = 2;
const int DOWN_KEY_PIN = 3;
const int DEBOUNCE_TIME = 30;

uint32_t actTime = 0;

volatile int16_t parameter = 0;
volatile int16_t lastp = 0;
volatile int32_t lastUpKeyTime = 0;
volatile int32_t lastDownKeyTime = 0;

// LOW = 0x0 = 0
// HIGH = 0x1 = 1
volatile int16_t upKeyState = LOW; 
volatile int16_t downKeyState = LOW;

void setup() {
  Serial.begin(9600);

  upKeyState = digitalRead(UP_KEY_PIN);
  downKeyState = digitalRead(DOWN_KEY_PIN);

  attachInterrupt(digitalPinToInterrupt(UP_KEY_PIN), upKeyPressed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_KEY_PIN), downKeyPressed, CHANGE);
  sei();
  Serial.println("Ready...");
}

void loop() {
  actTime = millis();

  if (lastp != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastp = parameter;
  }
  if (digitalRead(UP_KEY_PIN) == LOW && upKeyState == HIGH && actTime - lastUpKeyTime > DEBOUNCE_TIME) {
    upKeyState = LOW;
  }
  if (digitalRead(DOWN_KEY_PIN) == LOW && downKeyState == HIGH && actTime - lastDownKeyTime > DEBOUNCE_TIME) {
    downKeyState = LOW;
  }
}

void upKeyPressed() {
  uint32_t currentTime = millis();
  if (upKeyState == LOW && currentTime - lastUpKeyTime > DEBOUNCE_TIME) {
    parameter++;
    upKeyState = HIGH;
  } else {
    upKeyState = LOW;
  }
  lastUpKeyTime = currentTime;
}

void downKeyPressed() {
  uint32_t currentTime = millis();
  if (downKeyState == LOW && currentTime - lastDownKeyTime > DEBOUNCE_TIME) {
    parameter--;
    downKeyState = HIGH;
  } else {
    upKeyState = LOW;
  }
  lastDownKeyTime = currentTime;
}