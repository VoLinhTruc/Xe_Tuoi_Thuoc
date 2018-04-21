#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"
#include "Bico_STM8_Wire.h"

#define TIMER 100 //time-out cho moi hanh dong cua I2C
int timer = TIMER; // in us

#define LENGTH 2 // do dai du lieu can test
int length = LENGTH;


uint8_t reset = 0; // neu =1 thi reset I2C module, neu =0 thi thoi

void setup()
{
  Serial.begin(57600);
  enableGlobalInterrupt();
  Wire.begin();
}


void loop()
{
  Wire.beginTransmission(8);
  Wire.write(49);
  char a[] = "234\0";
  Wire.write("999");
  Wire.write(a, 2);
  Wire.write(52);
  Wire.endTransmission();
  
  delay_ms(2000);
}
