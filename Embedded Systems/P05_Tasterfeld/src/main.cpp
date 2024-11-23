#include <Arduino.h>

// Praktikum P05:
// Tasterfeld

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define C1 6
#define C2 7
#define C3 8
#define C4 9
#define R1 2
#define R2 3
#define R3 4
#define R4 5

#define PIN_OFFSET 2

#define STATE(input) (input == HIGH) ? "|1 " : "|0 "

#define ROWS 4
#define COLS 4
#define INDEX(currentCol) (currentRow * ROWS) + currentCol

volatile uint32_t time_current = 0;
volatile uint32_t last_display_time = 0;
const uint32_t cycle_time_display = 1000;

volatile uint8_t currentRow = 0;
volatile int16_t parameter;
volatile uint8_t inputMatrix[ROWS * COLS] = {0};
volatile uint8_t previousInputMatrix[ROWS * COLS] = {0};

void setup() {
  TCCR1A = 0; 
  // Todo Setup Timer (Zählvariable bis 65535), Taktfrequenz Prozessor 16 MHz
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = (1 << CS10) | (1 << CS11); 
  // 1 Sek TCCR1B = (1 << CS12); 
  sei(); 

  Serial.begin(9600);
  Serial.println("Ready...");
}

void printMatrix()
{
  Serial.println("  |C1|C2|C3|C4");
  Serial.println("--+--+--+--+--");
  Serial.print("R1");
  Serial.print(STATE(inputMatrix[0]));
  Serial.print(STATE(inputMatrix[1]));
  Serial.print(STATE(inputMatrix[2]));
  Serial.println(STATE(inputMatrix[3]));
  Serial.println("--+--+--+--+--");
  Serial.print("R2");
  Serial.print(STATE(inputMatrix[4]));
  Serial.print(STATE(inputMatrix[5]));
  Serial.print(STATE(inputMatrix[6]));
  Serial.println(STATE(inputMatrix[7]));
  Serial.println("--+--+--+--+--");
  Serial.print("R3");
  Serial.print(STATE(inputMatrix[8]));
  Serial.print(STATE(inputMatrix[9]));
  Serial.print(STATE(inputMatrix[10]));
  Serial.println(STATE(inputMatrix[11]));
  Serial.println("--+--+--+--+--");
  Serial.print("R4");
  Serial.print(STATE(inputMatrix[12]));
  Serial.print(STATE(inputMatrix[13]));
  Serial.print(STATE(inputMatrix[14]));
  Serial.println(STATE(inputMatrix[15]));

  Serial.print("Parameter: ");
  Serial.println(parameter);
  Serial.println();
}

void loop()
{
  time_current = millis();
  if (time_current > last_display_time + cycle_time_display) {
    last_display_time += cycle_time_display;
    printMatrix();
  }

  for (uint8_t i = 0; i < (ROWS * COLS); i++)
  {
    previousInputMatrix[i] = inputMatrix[i];
  }
}

ISR(TIMER1_OVF_vect)
{
  // Hier alle 10 ms reinspringen.
  // Taster auslesen, Zustandsautomat
  switch (currentRow + PIN_OFFSET)
  {
  case R1:
    pinMode(R1, OUTPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R1, HIGH);
    break;
  case R2:
    pinMode(R1, INPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R2, HIGH);
    break;
  case R3:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, INPUT);
    digitalWrite(R3, HIGH);
    break;
  case R4:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, OUTPUT);
    digitalWrite(R4, HIGH);
    break;
  default:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    break;
  }

  inputMatrix[INDEX(0)] = digitalRead(C1);
  inputMatrix[INDEX(1)] = digitalRead(C2);
  inputMatrix[INDEX(2)] = digitalRead(C3);
  inputMatrix[INDEX(3)] = digitalRead(C4);

  if (inputMatrix[0] == LOW && previousInputMatrix[0] == HIGH)
  {
    parameter++;
  }

  if (inputMatrix[1] == LOW && previousInputMatrix[1] == HIGH)
  {
    parameter--;
  }

  currentRow = (currentRow + 1) % 4;
}