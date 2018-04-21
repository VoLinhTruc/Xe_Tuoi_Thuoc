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
  Serial.write(analogRead(AIN2));
  delay_ms(100);
}
