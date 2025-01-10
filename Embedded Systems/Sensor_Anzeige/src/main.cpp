#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <avr/interrupt.h>

#define U_REF 5
#define R1 1000.0
#define AD_MAX 1023.0
#define MIN_TEMP_KTY120 -55;
#define MAX_TEMP_KTY120 155;

#define KTY120 0
#define LM35 1
#define MAGNET 2
#define LIGHT 3
#define SENSOR_NUMBER 4

#define KTY120_TEMP_SENSOR A0
#define LM35_TEMP_SENSOR A2
#define SS490_MAGNET_SENSOR A1
#define PDIC144_LIGHT_SENSOR A3

const float lookup[24][2] = {
    {-55, 490},
    {-50, 515},
    {-40, 567},
    {-30, 624},
    {-20, 684},
    {-10, 747},
    {0, 815},
    {10, 886},
    {20, 961},
    {25, 1000},
    {30, 1040},
    {40, 1122},
    {50, 1209},
    {60, 1299},
    {70, 1392},
    {80, 1490},
    {90, 1591},
    {100, 1696},
    {110, 1805},
    {120, 1915},
    {125, 1970},
    {130, 2023},
    {140, 2124},
    {150, 2211},
};

volatile uint16_t timerStart;

volatile uint8_t sensorSelector = KTY120;
volatile uint8_t prevPinState = LOW;

#define SELECT_NEXT_SENSOR sensorSelector = (sensorSelector + 1) % SENSOR_NUMBER

float getKTY120TempInCelsius();
float getLM35TempInCelsius();
float getSS490MagnetInGauss();
float getPDIC144LightInLux();

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  TCCR1A = 0;
  // Interrupt f체r Timer 1 aktivieren
  TIMSK1 |= (1 << TOIE1);
  // Prescaler = 64
  TCCR1B = (1 << CS10) | (1 << CS11);
  // 1 Sek TCCR1B = (1 << CS12);
  timerStart = 58036;
  TCNT1 = timerStart;
  Serial.begin(9600);
  sei();

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (sensorSelector)
  {
  case KTY120:
    if (getKTY120TempInCelsius() != -999.0)
    {
      lcd.print("KTY120: ");
      lcd.print(getKTY120TempInCelsius(), 1);
      lcd.print(char(223));
      lcd.print("C");
    }
    else
    {
      lcd.print("Error: KTY120");
    }
    break;
  case LM35:
    if (getLM35TempInCelsius() != -999.0)
    {
      lcd.print("LM35: ");
      lcd.print(getLM35TempInCelsius(), 1);
      lcd.print(char(223));
      lcd.print("C");
    }
    else
    {
      lcd.print("Error: LM35");
    }
    break;
  case MAGNET:

    if (getSS490MagnetInGauss() != -999.0)
    {
      lcd.print("SS490: ");
      lcd.print(getSS490MagnetInGauss(), 1);
      lcd.print("Gauss");
    }
    else
    {
      lcd.print("Error: SS490");
    }
    break;
  case LIGHT:
    if(getPDIC144LightInLux() != -999.0)
    {
      lcd.print("PDIC144: ");
      lcd.print(getPDIC144LightInLux(), 1);
      lcd.print("Lux");
    }
    else {
      lcd.print("Error: PDIC144");
    }
    break;
  }
  delay(1000);
}

float getKTY120TempInCelsius()
{
  float u_sensor = analogRead(KTY120_TEMP_SENSOR) * (U_REF / AD_MAX);
  float r_sensor = R1 * (U_REF - u_sensor) / u_sensor;

  // lcd.setCursor(0,1);
  // lcd.print("r_sensor: ");
  // lcd.print(r_sensor);
  // lcd.setCursor(8,0);

  float t_1, t_2, r_1, r_2;

  for (int i = 0; i < 24; i++)
  {
    if (r_sensor >= lookup[i][1] && r_sensor <= lookup[i + 1][1])
    {
      t_1 = lookup[i][0];
      t_2 = lookup[i + 1][0];
      r_1 = lookup[i][1];
      r_2 = lookup[i + 1][1];

      return t_1 + (r_sensor - r_1) * (t_2 - t_1) / (r_2 - r_1);
    }
  }
  return -999.0;
}

float getLM35TempInCelsius()
{
  float u_sensor = analogRead(LM35_TEMP_SENSOR) * (U_REF / AD_MAX);
  float temp = u_sensor * 10.0;

  if(temp >= -55 && temp <= 150)
  {
    return temp;
  }
  return -999.0;
}

float getSS490MagnetInGauss()
{
  return -999.0;
}

float getPDIC144LightInLux()
{
  return -999.0;
}

ISR(TIMER1_OVF_vect)
{
  uint8_t currentPinState = digitalRead(2);

  if (currentPinState == LOW && prevPinState == HIGH)
  {
    SELECT_NEXT_SENSOR;
  }

  prevPinState = currentPinState;
}