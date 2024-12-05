#include <Arduino.h>

// Praktikum P05:
// Tasterfeld

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

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

#define PLUS 0x0
#define MINUS 0x1

#define NONE 0x0
#define PRINT 0x1
#define READ 0x2
#define WRITE 0x3

// lcd display
const static int LCD_I2C_ADDRESS = 0x27;
const static int LCD_COLS = 16;
const static int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

#define COPY_INPUT_MATRIX_TO_PREVIOUS         \
  for (uint8_t i = 0; i < (ROWS * COLS); i++) \
    previousInputMatrix[i] = inputMatrix[i];

volatile uint32_t time_current = 0;
volatile uint32_t last_display_time = 0;
const uint32_t cycle_time_display = 1000;

volatile uint8_t currentRow = 0;
volatile uint8_t inputMatrix[ROWS * COLS] = {0};
volatile uint8_t previousInputMatrix[ROWS * COLS] = {0};

volatile int8_t p0 = 0;
volatile int8_t p1 = 0;
volatile int8_t p2 = 0;
volatile int8_t p3 = 0;
volatile int8_t memoryAction = NONE;

volatile int8_t printStatus = NONE;
volatile int8_t writeStatus = NONE;
volatile int8_t readStatus = NONE;

volatile int8_t *pointerP0 = &p0;
volatile int8_t *pointerP1 = &p1;
volatile int8_t *pointerP2 = &p2;
volatile int8_t *pointerP3 = &p3;

#define STATE(input) (input == HIGH) ? "|1 " : "|0 "

#define INDEX(currentCol) (currentRow * ROWS) + currentCol

#define CHANGE_VALUE(buttonId, currentParameter, operator)                        \
  if (inputMatrix[buttonId] == HIGH && previousInputMatrix[buttonId] == LOW)      \
  {                                                                               \
    (*currentParameter) = (*currentParameter) + ((operator== PLUS) ? 1 : -1 * 1); \
  }

#define PRINT_PARAMS  \
  lcd.print(p0);      \
  lcd.print(" "); \
  lcd.print(p1);      \
  lcd.print(" "); \
  lcd.print(p2);      \
  lcd.print(" "); \
  lcd.print(p3);

#define CLEAR_INPUT_LINE(y) lcd.setCursor(0, y); lcd.print("                ");

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

  lcd.init();
  lcd.backlight();
  lcd.clear();

  PRINT_PARAMS;

  Serial.begin(9600);
  Serial.println("Ready...");
}

void loop()
{
  if (printStatus == PRINT)
  {
    CLEAR_INPUT_LINE(1);
    CLEAR_INPUT_LINE(0);
    lcd.setCursor(0, 0);
    PRINT_PARAMS;
    printStatus = NONE;
  }

  if (readStatus == READ)
  {
    CLEAR_INPUT_LINE(1);
    lcd.setCursor(0, 1);
    lcd.print("READING...");
    p0 = EEPROM.read(0);
    p1 = EEPROM.read(1);
    p2 = EEPROM.read(2);
    p3 = EEPROM.read(3);
    readStatus = NONE;
  }

  if (writeStatus == WRITE)
  {
    CLEAR_INPUT_LINE(1);
    lcd.setCursor(0, 1);
    lcd.print("WRITING...");
    EEPROM.put(0, p0);
    EEPROM.put(1, p1);
    EEPROM.put(2, p2);
    EEPROM.put(3, p3);
    writeStatus = NONE;
  }
}

ISR(TIMER1_COMPA_vect)
{
  // Hier alle 10 ms reinspringen.
  // Taster auslesen, Zustandsautomat
  COPY_INPUT_MATRIX_TO_PREVIOUS;
  
  switch (currentRow)
  {
  case 0:
    pinMode(R1, OUTPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R1, HIGH);
    break;
  case 1:
    pinMode(R1, INPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, INPUT);
    pinMode(R4, INPUT);
    digitalWrite(R2, HIGH);
    break;
  case 2:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, INPUT);
    digitalWrite(R3, HIGH);
    break;
  case 3:
    pinMode(R1, INPUT);
    pinMode(R2, INPUT);
    pinMode(R3, INPUT);
    pinMode(R4, OUTPUT);
    digitalWrite(R4, HIGH);
    break;
  }

  inputMatrix[INDEX(0)] = digitalRead(C1);
  inputMatrix[INDEX(1)] = digitalRead(C2);
  inputMatrix[INDEX(2)] = digitalRead(C3);
  inputMatrix[INDEX(3)] = digitalRead(C4);

  CHANGE_VALUE(S1, &p0, PLUS);
  CHANGE_VALUE(S2, &p1, PLUS);
  CHANGE_VALUE(S3, &p2, PLUS);
  CHANGE_VALUE(S4, &p3, PLUS);

  CHANGE_VALUE(S5, &p0, MINUS);
  CHANGE_VALUE(S6, &p1, MINUS);
  CHANGE_VALUE(S7, &p2, MINUS);
  CHANGE_VALUE(S8, &p3, MINUS);

  if (inputMatrix[S9] == LOW && previousInputMatrix[S9] == HIGH)
    printStatus = PRINT;

  if (inputMatrix[S10] == LOW && previousInputMatrix[S10] == HIGH)
    writeStatus = WRITE;

  if (inputMatrix[S11] == LOW && previousInputMatrix[S11] == HIGH)
    readStatus = READ;

  currentRow = (currentRow + 1) % ROWS;
}
