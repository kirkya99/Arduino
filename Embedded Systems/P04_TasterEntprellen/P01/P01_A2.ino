#include <avr/io.h>
#include <avr/interrupt.h>

// Praktikum P04: 
// Taster entprellen, Long Press 
// Version 2: Entprellen mit Timern - Polling in ISR


uint32_t lastDisplayTime = 0;
uint32_t cycleTimeDisplay = 300; 
// states : 
// 0 = nicht gedrückt
// 1 = gedrückt 
volatile int stateKeyUp = 0;
volatile int stateKeyDown = 0;

void setup() {
  TCCR1A = 0; 
  // Todo Setup Timer (Zählvariable bis 65535), Taktfrequenz Prozessor 16 MHz
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = (1 << CS10) | (1 << CS11); 
  // 1 Sek TCCR1B = (1 << CS12); 
 
  Serial.begin(9600);
  sei(); 
}

void loop() {
  actTime = millis(); 
  if (actTime > lastDisplayTime + cycleTimeDisplay) {
    lastDisplayTime += cycleTimeDisplay;
    Serial.print("Parameter"); 
    Serial.println(parameter); 
  }
}


ISR(TIMER1_OVF_vect)
{
    // Hier alle 30 ms reinspringen. 
    // Taster auslesen, Zustandsautomat
}