#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

float getKTY110Temp();

#define minTemp = -55;
#define maxTemp = 155;

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

void setup()
{
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
  if (getKTY110Temp() != -999.0)
  {
    lcd.setCursor(0, 0);
    lcd.print("KTY110: ");
    lcd.print(getKTY110Temp(), 1);
    lcd.print(char(223));
    lcd.print("C");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Error");
  }

  delay(1000);
}

float getKTY110Temp()
{
  float u_sensor = analogRead(A0) * (5.0 / 1023.0);
  float r_sensor = 1000 * (5.0 - u_sensor) / u_sensor;

  lcd.setCursor(0,1);
  lcd.print("r_sensor: ");
  lcd.print(r_sensor);
  lcd.setCursor(8,0);

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