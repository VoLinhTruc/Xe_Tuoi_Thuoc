#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

void setup()
{
  clkConfig();
  Serial.begin(115200);
  pinMode(PB5, OUTPUT);
}

void loop()
{
  if(Serial.available() > 0)
  {
    Serial.write(Serial.read());
  }
}