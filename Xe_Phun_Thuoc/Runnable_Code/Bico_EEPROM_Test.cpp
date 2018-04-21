#include "ArduinoStyle.h"

void setup()
{
  clkConfig();
  Serial.begin(57600);
  
  //Uncomment this block forr the first downloading program 
  /*
  EEPROM.write(1, 49);
  delay_ms(50);
  EEPROM.write(2, 50);
  delay_ms(50);
  EEPROM.write(3, 51);
  delay_ms(50);
  */
}

void loop()
{
  Serial.write(EEPROM.read(1));
  Serial.write('\t');
  Serial.write(EEPROM.read(2));
  Serial.write('\t');
  Serial.write(EEPROM.read(3));
  Serial.write('\n');
  Serial.write('\r');
  
  delay_ms(500);
}
