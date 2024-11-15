// Praktikum P04:
// Taster entDEBOUNCE_TIMEen, Long Press
// Version 2: EntDEBOUNCE_TIMEen mit Timern - Polling in ISR

#include <avr/io.h>
#include <avr/interrupt.h>

const int UP_KEY_PIN = 2;
const int DOWN_KEY_PIN = 3;
const int DEBOUNCE_TIME = 30;

const uint32_t LONG_PRESS = 1000;  // long press >= 1s

volatile uint32_t actTime = 0;
volatile uint32_t lastUpKeyTime = 0;
volatile uint32_t lastDownKeyTime = 0;
volatile uint32_t lastUpHoldTime = 0;
volatile uint32_t lastDownHoldTime = 0;
// states :
// 0 = nicht gedr체ckt
// 1 = gedr체ckt
volatile int stateKeyUp = 0;
volatile int stateKeyDown = 0;
volatile int16_t parameter;
volatile int16_t lastp;

void setup() {
  // Todo Setup Timer (Z채hlvariable bis 65535), Taktfrequenz Prozessor 16 MHz

  TCCR1A = 0;
  // Interrupt f체r Timer 1 aktivieren
  TIMSK1 |= (1 << TOIE1);
  // Prescaler = 64
  TCCR1B = (1 << CS10) | (1 << CS11);
  // 1 Sek TCCR1B = (1 << CS12);

  Serial.begin(9600);
  sei();
}

void loop() {
  actTime = millis();

  if (lastp != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastp = parameter;
  }

  if (digitalRead(UP_KEY_PIN) == HIGH && stateKeyUp == 0) {
    if (actTime - lastUpKeyTime > DEBOUNCE_TIME) {
      lastUpKeyTime = actTime;
      stateKeyUp = 1;
    }
  } else if (digitalRead(UP_KEY_PIN) == HIGH && stateKeyUp == 1) {
    if (actTime - lastUpKeyTime >= LONG_PRESS) {
      stateKeyUp = 2;
      lastUpHoldTime = actTime;
    }
  }

  if (digitalRead(DOWN_KEY_PIN) == HIGH && stateKeyDown == 0) {
    if (actTime - lastDownKeyTime > DEBOUNCE_TIME) {
      lastDownKeyTime = actTime;
      stateKeyDown = 1;
    }
  } else if (digitalRead(DOWN_KEY_PIN) == HIGH && stateKeyDown == 1) {
    if (actTime - lastDownKeyTime >= LONG_PRESS) {
      stateKeyDown = 2;
      lastDownHoldTime = actTime;
    }
  }
}

ISR(TIMER1_OVF_vect) {
  // Hier alle 30 ms reinspringen.
  // Taster auslesen, Zustandsautomat

  int32_t pressedUpDuration = actTime - lastUpHoldTime;
  int32_t pressedDownDuration = actTime - lastDownHoldTime;

  // up key
  if (digitalRead(2) == LOW) {
    if (stateKeyUp == 1) {
      parameter += 1;
    }
    stateKeyUp = 0;
  } else if (digitalRead(2) == HIGH && stateKeyUp == 2 && pressedUpDuration > LONG_PRESS) {
    parameter += 10;
    lastUpHoldTime = actTime;
  }

  // down key
  if (digitalRead(3) == LOW) {
    if (stateKeyDown == 1) {
      parameter -= 1;
    }
    stateKeyDown = 0;
  } else if (digitalRead(3) == HIGH && stateKeyDown == 2 && pressedDownDuration > LONG_PRESS) {
      parameter -= 10;
      lastDownHoldTime = actTime;
  }
}