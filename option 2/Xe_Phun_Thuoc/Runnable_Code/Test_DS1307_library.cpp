#include "ArduinoStyle.h"
#include "Bico_STM8_Wire.h"
#include "DS1307.h"

#define delay delay_ms


DS1307 clock;
RTCDateTime dt;

void setup()
{
  Serial.begin(115200);

  clock.begin();

  // If date not set
  if (!clock.isReady())
  {
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);
  }
}

void loop()
{
  dt = clock.getDateTime();

  // For leading zero look to DS1307_dateformat example

  Serial.write(dt.hour);   
  Serial.write(dt.minute);   
  Serial.write(dt.second); 

  delay(1000);
}