#include <avr/interrupt.h>

const int ledPin = 13;
volatile bool isLedOn = false; 
volatile bool isCycle = false; 
uint16_t cycleNo = 0;
uint32_t actTime = 0; 
uint32_t lastDisplayTime = 0;
uint32_t cycleTimeDisplay = 300; 

uint32_t lastUpKeyTime = 0;
uint32_t lastDownKeyTime = 0; 


void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), upKeyPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(3), downKeyPressed, RISING);
  sei(); 
}

// Praktikum P04: 
// Taster entprellen, Long Press 
// Version 1: Entprellen mit externen Interrupts und millis()

volatile int16_t parameter;
volatile int16_t lastp = 0;
bool isUpKeyRising = false;
// bool isDownKeyRising = false;

void loop() {
  actTime = millis(); 
  
  if (lastp != parameter) {
    Serial.print("Parameter"); 
    Serial.println(parameter);
    lastp = parameter;
  }
}

void upKeyPressed() {
  uint32_t currentTime = millis();
    if (currentTime - lastUpKeyTime > 30) {
      lastp = parameter;
      parameter++;
    }
  lastUpKeyTime = currentTime;
}

void downKeyPressed()
{
  uint32_t currentTime = millis();
  if (currentTime - lastDownKeyTime > 30) {
    lastp = parameter;
    parameter--;
  }
  lastDownKeyTime = currentTime;
}