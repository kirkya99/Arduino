#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void upKeyPressed();
void downKeyPressed();

typedef enum buttonState {
  RELEASED = 0x0,
  PRESSED = 0x1
};

const uint8_t UP_KEY_PIN = 2;
const uint8_t DOWN_KEY_PIN = 3;
const uint8_t UP_KEY_PIN_ID = PD2;
const uint8_t DOWN_KEY_PIN_ID = PD3;

const uint8_t DEBOUNCE = 30;
volatile uint32_t actTime = 0;
uint32_t lastUpKeyTime = 0;
uint32_t lastDownKeyTime = 0;
volatile uint8_t upKeyState = RELEASED;
volatile uint8_t downKeyState = RELEASED;
volatile int16_t parameter;
int16_t lastParameter;

void upKeyPressed();
void downKeyPressed();

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(UP_KEY_PIN), upKeyPressed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_KEY_PIN), downKeyPressed, CHANGE);
  sei();
  DDRD &= ~(1 << UP_KEY_PIN_ID);
  DDRD &= ~(1 << DOWN_KEY_PIN_ID);
  Serial.println("Ready...");
}

// Praktikum P04: 
// Taster entprellen, Long Press 
// Version 1: Entprellen mit externen Interrupts und millis()

void loop() {
  actTime = millis();

  if (lastParameter != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastParameter = parameter;
  }
}

void upKeyPressed() {
  if (actTime - lastUpKeyTime > DEBOUNCE) {
    uint8_t currentUpKeyState = PIND & (1 << UP_KEY_PIN_ID) ? PRESSED : RELEASED;
    if(upKeyState == RELEASED && currentUpKeyState == PRESSED) {
      upKeyState = PRESSED;
      parameter++;
    } else if (upKeyState == PRESSED && currentUpKeyState == RELEASED) {
      upKeyState = RELEASED;
    }
  }
}

void downKeyPressed() {
  if (actTime - lastDownKeyTime > DEBOUNCE) {
    bool currentDownKeyState = PIND & (1 << DOWN_KEY_PIN_ID);
    if(downKeyState == RELEASED && currentDownKeyState == PRESSED) {
      downKeyState = PRESSED;
      parameter--;
    } else if (downKeyState == PRESSED && currentDownKeyState == RELEASED) {
      downKeyState = RELEASED;
    }
  }
}