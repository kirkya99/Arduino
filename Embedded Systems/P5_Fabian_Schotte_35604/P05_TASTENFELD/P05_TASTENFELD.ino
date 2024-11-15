#include <avr/io.h>
#include <avr/interrupt.h>

// Praktikum P05: 
// Tasterfeld


uint32_t lastDisplayTime = 0;
uint32_t cycleTimeDisplay = 300; 
// states : 
// 0 = nicht gedrückt
// 1 = gedrückt 
volatile int stateKeyUp = 0;
volatile int stateKeyDown = 0;
int16_t lastParameter;


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
}