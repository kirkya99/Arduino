#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define minTemp = -55;
#define maxTemp = 155;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  
}