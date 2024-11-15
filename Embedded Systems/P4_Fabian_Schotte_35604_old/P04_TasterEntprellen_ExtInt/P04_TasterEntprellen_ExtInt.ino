#include <avr/io.h>
#include <avr/interrupt.h>

const int UP_KEY_PIN = 2;
const int DOWN_KEY_PIN = 3;
const int DEBOUNCE_TIME = 30;

volatile uint32_t actTime = 0;
uint32_t lastUpKeyTime = 0;
uint32_t lastDownKeyTime = 0;
volatile int stateKeyUp = 0;
volatile int stateKeyDown = 0;
volatile int16_t parameter;
int16_t lastp;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(UP_KEY_PIN), upKeyPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(DOWN_KEY_PIN), downKeyPressed, RISING);
  sei();
  Serial.println("Ready...");
}

void loop() {
  actTime = millis();

  int keyUp = digitalRead(UP_KEY_PIN);
  int keyDown = digitalRead(DOWN_KEY_PIN);

  if (stateKeyUp == 1 && keyUp == 1 && actTime - lastUpKeyTime > DEBOUNCE_TIME) {
    parameter++;
    stateKeyUp = 0;
  }

  if (stateKeyDown == 1 && keyDown == 1 && actTime - lastDownKeyTime > DEBOUNCE_TIME) {
    parameter--;
    stateKeyDown = 0;
  }

  if (lastp != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastp = parameter;
  }
}

void upKeyPressed() {
  if (actTime - lastUpKeyTime > DEBOUNCE_TIME) {
    stateKeyUp = 1;
    lastUpKeyTime = actTime;
  }
}

void downKeyPressed() {
  if (actTime - lastDownKeyTime > DEBOUNCE_TIME) {
    stateKeyDown = 1;
    lastDownKeyTime = actTime;
  }
}