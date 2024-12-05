#include <Arduino.h>

// Praktikum P05:
// Tasterfeld

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Columns
#define C1 6
#define C2 7
#define C3 8
#define C4 9
// Rows
#define R1 5
#define R2 4
#define R3 3
#define R4 2

#define ROWS 4
#define COLS 4

#define ROW0 0
#define ROW1 1
#define ROW2 2
#define ROW3 3
#define COL0 0
#define COL1 1
#define COL2 2
#define COL3 3

#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define S5 4
#define S6 5
#define S7 6
#define S8 7
#define S9 8
#define S10 9
#define S11 10
#define S12 11
#define S13 12
#define S14 13
#define S15 14
#define S16 15

#define STATE(input) (input == HIGH) ? "|1 " : "|0 "

#define PRINT_MATRIX                       \
  Serial.println("  |C1|C2|C3|C4");        \
  Serial.println("--+--+--+--+--");        \
  Serial.print("R1");                      \
  Serial.print(STATE(inputMatrix[S1]));    \
  Serial.print(STATE(inputMatrix[S2]));    \
  Serial.print(STATE(inputMatrix[S3]));    \
  Serial.println(STATE(inputMatrix[S4]));  \
  Serial.println("--+--+--+--+--");        \
  Serial.print("R2");                      \
  Serial.print(STATE(inputMatrix[S5]));    \
  Serial.print(STATE(inputMatrix[S6]));    \
  Serial.print(STATE(inputMatrix[S7]));    \
  Serial.println(STATE(inputMatrix[S8]));  \
  Serial.println("--+--+--+--+--");        \
  Serial.print("R3");                      \
  Serial.print(STATE(inputMatrix[S9]));    \
  Serial.print(STATE(inputMatrix[S10]));   \
  Serial.print(STATE(inputMatrix[S11]));   \
  Serial.println(STATE(inputMatrix[S12])); \
  Serial.println("--+--+--+--+--");        \
  Serial.print("R4");                      \
  Serial.print(STATE(inputMatrix[S13]));   \
  Serial.print(STATE(inputMatrix[S14]));   \
  Serial.print(STATE(inputMatrix[S15]));   \
  Serial.println(STATE(inputMatrix[S16])); \
  Serial.print("Parameter: ");             \
  Serial.println(parameter);               \
  Serial.println();

#define INDEX(currentCol) (currentRow * ROWS) + currentCol

#define COPY_INPUT_MATRIX_TO_PREVIOUS          \
  for (uint8_t i = 0; i < (ROWS * COLS); i++)  \
    previousInputMatrix[i] = inputMatrix[i];   \

volatile uint32_t time_current = 0;
volatile uint32_t last_display_time = 0;
const uint32_t cycle_time_display = 1000;

volatile uint8_t currentRow = 0;
volatile int16_t parameter;
volatile uint8_t inputMatrix[ROWS * COLS] = {0};
volatile uint8_t previousInputMatrix[ROWS * COLS] = {0};

void setup()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);
  OCR1A = 2499;                        // 16 MHz / 64 (Prescaler) = 250000 Ticks/s => 2500 Ticks/10ms - 1
  TIMSK1 |= (1 << OCIE1A);             // Enable Timer1 compare interrupt
  TCCR1B |= (1 << CS11) | (1 << CS10); // Start Timer1 with prescaler 64

  pinMode(C1, INPUT);
  pinMode(C2, INPUT);
  pinMode(C3, INPUT);
  pinMode(C4, INPUT);

  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);

  sei();

  Serial.begin(9600);
  Serial.println("Ready...");
}

void loop()
{
  time_current = millis();
  if (time_current > last_display_time + cycle_time_display)
  {
    last_display_time += cycle_time_display;
    PRINT_MATRIX
  }
}

ISR(TIMER1_COMPA_vect)
{
  // Hier alle 10 ms reinspringen.
  // Taster auslesen, Zustandsautomat
  COPY_INPUT_MATRIX_TO_PREVIOUS
  
  switch (currentRow)
  {
  case ROW0:
    pinMode(R1, OUTPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R1, HIGH);
    break;
  case ROW1:
    pinMode(R1, INPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R2, HIGH);
    break;
  case ROW2:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, INPUT);
    digitalWrite(R3, HIGH);
    break;
  case ROW3:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, OUTPUT);
    digitalWrite(R4, HIGH);
    break;
  }

  inputMatrix[INDEX(COL0)] = digitalRead(C1);
  inputMatrix[INDEX(COL1)] = digitalRead(C2);
  inputMatrix[INDEX(COL2)] = digitalRead(C3);
  inputMatrix[INDEX(COL3)] = digitalRead(C4);

  if (inputMatrix[S1] == LOW && previousInputMatrix[S1] == HIGH)
  {
    parameter++;
  }

  if (inputMatrix[S2] == LOW && previousInputMatrix[S2] == HIGH)
  {
    parameter--;
  }

  currentRow = (currentRow + 1) % ROWS;
}