#include <avr/io.h>
#include <avr/interrupt.h>

const int UP_KEY_PIN = 2;
const int DOWN_KEY_PIN = 3;
const int DEBOUNCE = 30;
volatile uint32_t actTime = 0;
uint32_t lastUpKeyTime = 0;
uint32_t lastDownKeyTime = 0;
volatile uint8_t stateKeyUp = LOW;
volatile uint8_t stateKeyDown = LOW;
volatile int16_t parameter;
int16_t lastParameter;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(UP_KEY_PIN), upKeyPressed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_KEY_PIN), downKeyPressed, CHANGE);
  sei();
  DDRD &= ~(1 << PD2);
  DDRD &= ~(1 << PD3);
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
    bool currentUpKeyState = PIND & (1 << PD2);
    if(stateKeyUp == LOW && currentUpKeyState == HIGH) {
      stateKeyUp = HIGH;
      parameter++;
    } else if (stateKeyUp == HIGH && currentUpKeyState == LOW) {
      stateKeyUp = LOW;
    }
  }
}

void downKeyPressed() {
  if (actTime - lastDownKeyTime > DEBOUNCE) {
    bool currentDownKeyState = PIND & (1 << PD3);
    if(stateKeyDown == LOW && currentDownKeyState == HIGH) {
      stateKeyDown = HIGH;
      parameter--;
    } else if (stateKeyDown == HIGH && currentDownKeyState == LOW) {
      stateKeyDown = LOW;
    }
  }
}