#include <avr/io.h>
#include <avr/interrupt.h>

// Praktikum P04: 
// Taster entprellen, Long Press 
// Version 2: Entprellen mit Timern - Polling in ISR

typedef enum buttonState {
  RELEASED = 0x0,
  PRESSED = 0x1
};

const uint8_t UP_KEY_PIN = PD2;
const uint8_t DOWN_KEY_PIN = PD3;
const uint8_t LONG_PRESS_ITERATIONS = 33; // ISR called every 30ms: 30ms * 33ms = 990ms

// states :
// 0 = nicht gedrückt
// 1 = gedrückt
volatile uint8_t currentUpKeyPressedIterations = 0;
volatile uint8_t currentDownKeyPressedIterations = 0;
volatile int16_t parameter = 0;
volatile int16_t lastParameter;
bool statePinUp = RELEASED;
bool statePinDown = RELEASED;
uint8_t longpressState = RELEASED;
volatile int lastLong = 0;
volatile uint16_t timerStart = 0;

void setup() {
  TCCR1A = 0; 
  // Todo Setup Timer (Zählvariable bis 65535), Taktfrequenz Prozessor 16 MHz
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = (1 << CS10) | (1 << CS11); 
  // 1 Sek TCCR1B = (1 << CS12); 
  timerStart = 58036;
  TCNT1 = timerStart;
  DDRD &= ~(1 << UP_KEY_PIN);
  DDRD &= ~(1 << DOWN_KEY_PIN);

  Serial.begin(9600);
  sei();
  Serial.println("Ready...");
}

void loop() {
  if (lastParameter != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastParameter = parameter;
  }
}

ISR(TIMER1_OVF_vect) {
  // Hier alle 30 ms reinspringen.
  // Taster auslesen, Zustandsautomat

  uint8_t currentUpPinState = PIND & (1 << UP_KEY_PIN) ? PRESSED : RELEASED;
  uint8_t currentDownPinState = PIND & (1 << DOWN_KEY_PIN) ? PRESSED : RELEASED;

  // Increment of the parameter
  if (currentUpPinState == PRESSED && statePinUp == HIGH) {
    if (currentUpKeyPressedIterations > LONG_PRESS_ITERATIONS) {
      parameter = parameter + 10;
      currentUpKeyPressedIterations = 0;
      longpressState = PRESSED;
    }
    currentUpKeyPressedIterations++;
  } else if (currentUpPinState == RELEASED && statePinUp == HIGH && currentUpKeyPressedIterations == 0) {
    if (longpressState != PRESSED) {
      parameter++;
    }
    longpressState = RELEASED;

    statePinUp = RELEASED;
  } else if (currentUpPinState == HIGH && statePinUp == LOW) {
    statePinUp = PRESSED;
  } else {
    currentUpKeyPressedIterations = 0;
  }

  // Decrement of the parameter
  if (currentDownPinState == HIGH && statePinDown == HIGH) {
    if (currentDownKeyPressedIterations > LONG_PRESS_ITERATIONS) {  // 990 ms
      parameter -= 10;
      currentDownKeyPressedIterations = 0;
      longpressState = PRESSED;
    }
    currentDownKeyPressedIterations++;
  } else if (currentDownPinState == LOW && statePinDown == HIGH && currentDownKeyPressedIterations == 0) {
    if (longpressState != PRESSED) {
      parameter--;
    }
    longpressState = RELEASED;

    statePinDown = RELEASED;
  } else if (currentDownPinState == HIGH && statePinDown == LOW) {
    statePinDown = PRESSED;
  } else {
    currentDownKeyPressedIterations = 0;
  }
  TCNT1 = timerStart;
}