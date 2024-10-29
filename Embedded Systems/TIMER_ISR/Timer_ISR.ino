#include <avr/io.h>
#include <avr/interrupt.h>


const int ledPin = 13;
volatile int seconds = 0;

void setup() {

  pinMode(ledPin, OUTPUT);
  seconds = 0;
  TCCR1A = 0; 
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = (1 << CS12); 
 
  Serial.begin(9600);
  sei(); 
}

void loop() {
  
}

ISR(TIMER1_OVF_vect)
{
  Serial.print(seconds); 
  if (seconds > 2) {
      digitalWrite(ledPin, !digitalRead(ledPin));
      Serial.print(" LED toggeln\n"); 
      seconds = 0;
  } else {
    seconds++;
  } 

  
}

