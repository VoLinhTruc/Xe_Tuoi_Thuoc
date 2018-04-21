#include "ArduinoStyle.h"
#include "Bico_STM8_Wire.h"

#define NUM_BYTE 6

void setup()
{
  Serial.begin(57600);
  Wire.begin();
}


void loop()
{
  Wire.requestFrom(8, 8);
  while(Wire.available() > 0)
  {
    Serial.write(Wire.read());
  }
  Serial.println();
  
  delay_ms(3000);
}
