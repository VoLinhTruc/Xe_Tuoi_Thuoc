#include "ArduinoStyle.h"
#include "Bico_STM8_Spi.h"

char c = 0;

void setup()
{
  Serial.begin(57600);
  
  pinMode(PB5, OUTPUT);
  pinMode(PD3, OUTPUT);
}



void loop()
{
  digitalWriteReverse(PB5);
  delay_ms(500);
  
  digitalWrite(PD3, 0);
  SPI.beginTransaction();
  Serial.write(SPI.transfer(c++));
  SPI.endTransaction();
  digitalWrite(PD3, 1);
}
