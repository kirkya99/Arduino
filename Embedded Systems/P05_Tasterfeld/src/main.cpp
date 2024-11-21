#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Praktikum P05: 
// Tasterfeld

const uint8_t R1 = 0;
const uint8_t R2 = 1;
const uint8_t R3 = 2;
const uint8_t R4 = 3;
const uint8_t C1 = 4;
const uint8_t c2 = 5;
const uint8_t C3 = 6;
const uint8_t C4 = 7;


#define ROWS 4
#define COLS 4
volatile uint8_t currentRow = 0;

// states : 
// 0 = nicht gedrückt
// 1 = gedrückt 
volatile int stateKeyUp = 0;
volatile int stateKeyDown = 0;
volatile int16_t parameter;
volatile int16_t lastParameter;
// put function declarations here:

void setup() {
  TCCR1A = 0; 
  // Todo Setup Timer (Zählvariable bis 65535), Taktfrequenz Prozessor 16 MHz
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = (1 << CS10) | (1 << CS11); 
  // 1 Sek TCCR1B = (1 << CS12); 
  sei(); 

  Serial.begin(9600);
}


void loop() {
if (lastParameter != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
    lastParameter = parameter;
  }
}

ISR(TIMER1_OVF_vect)
{
  // Hier alle 30 ms reinspringen. 
  // Taster auslesen, Zustandsautomat
  switch (currentRow)
  {
    case R1:

      break;
    case R2:

      break;
    case R3:

      break;
    case R4:

      break;
    default:
      break;
  }


  currentRow = currentRow + 1 % 4;
}