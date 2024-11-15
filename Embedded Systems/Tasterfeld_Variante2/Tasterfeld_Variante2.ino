// Praktikum 02:
// Tastenfeld - Multiplexing

const int PIN_C1 = 0;
const int PIN_C2 = 1;
const int PIN_C3 = 2;
const int PIN_C4 = 3;
const int PIN_R1 = 4;
const int PIN_R2 = 5;
const int PIN_R3 = 6;
const int PIN_R4 = 7;

volatile int16_t currentRow = 0;
volatile int16_t states[12] = { 0 };

volatile int16_t parameter = 0;
int16_t lastP = 0;

void setup() {
  TCCR1A = 0;
  // CTC mode, Prescaler 64
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
  // compare match register (10 ms)
  OCR1A = 2499;
  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  DDRD &= ~0x0F;  // Setzt D0-D7 als Eingang
  PORTD |= 0x0F;  // Aktiviert die Pull-up-Widerstände für D0-D7

  // DDRD &= ~(1 << PD4);

  pinMode(PIN_R1, INPUT);

  Serial.begin(9600);
  sei();
  Serial.println("Ready...");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (lastP != parameter) {
    Serial.print("Parameter: ");
    Serial.println(parameter);
  }

}

ISR(TIMER1_COMPA_vect) {
  
  // Einlesen 
  // bool curr = PIND & (1 << PD0);
  bool c1 = PIND & (1 << PD0);
  bool r1 = PIND & (1 << PD4);

  // if (curr == 0) {
  //   parameter++;
  // }

  if(c1 == HIGH) {
    parameter++;
  }
  
  currentRow++;
}