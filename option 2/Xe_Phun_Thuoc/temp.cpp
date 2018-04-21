
#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"
#include "Bico_STM8_Wire.h"

//#define WAITING_TIMEOUT 100 //time-out cho moi hanh dong cua I2C

#define LENGTH 2 // do dai du lieu can test
int length = LENGTH;

int recv_length = 3;

uint8_t txAddress = 8;
uint8_t reset = 0; // neu =1 thi reset I2C module, neu =0 thi thoi

void setup()
{
  Serial.begin(57600);
  enableGlobalInterrupt();
  Wire.begin();
}


void loop()
{
  int count = 0;
  //START generation
  I2C_GenerateSTART(ENABLE);
  //wait until "count" reach "TIMER" value
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_GenerateSTART\r\n");
#endif
    if(count++ > WAITING_TIMEOUT)
    {
      break;
    }
  }

  //Send address and direction
  I2C_Send7bitAddress(txAddress<<1, I2C_DIRECTION_RX);
  count = 0;
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_Send7bitAddress\r\n");
#endif
    if(count++ > WAITING_TIMEOUT)
    {
      break;
    }
  }
  
  for(int i = 0; i < recv_length; i++)
  {
    while(I2C_GetLastEvent() != I2C_EVENT_MASTER_BYTE_RECEIVED)
    {
  #ifdef DEBUG_I2C
      //Moniting hanh dong hien tai cua I2C
      Serial.print("NO I2C_EVENT_MASTER_BYTE_RECEIVED\r\n");
  #endif
      if(count++ > WAITING_TIMEOUT)
      {
        break;
      }
    }

    Serial.write(I2C_ReceiveData());
  }

  I2C_GenerateSTOP(ENABLE);
  
  
 
  delay_ms(2000);
}
