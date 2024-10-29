int ledPin = 13;
int keyPin = 2; 
volatile bool state = false; 


void setup()
{
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), keyPressed, RISING);
}

uint32_t lastKeyPressed = 0;

void loop()
{
  digitalWrite(13,state); 

}

void keyPressed()
{
  state = !state; 
}
