#include "ArduinoStyle.h"

#define delay delay_ms

void setup()
{
  Serial.begin(57600);
}

char a = 0;
uint8_t b = 0;
int c = 0;
long d = 0;

void loop()
{
  Serial.print(a+=10);
  Serial.print(" ");
  Serial.print(b+=10);
  Serial.print(" ");
  Serial.print(c+=1000);
  Serial.print(" ");
  Serial.println(d+=500000000);
  
  delay(500);
}