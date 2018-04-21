#define FALSE 0
#define TRUE 1

#include "ArduinoStyle.h"

void main()
{
  
clkConfig();
#if defined BICO_STM8_EEPROM_H
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
#endif
  setup();
  while (1)
  {
    loop();
  }
}