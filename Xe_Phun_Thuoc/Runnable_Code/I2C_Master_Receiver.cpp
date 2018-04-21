#include "ArduinoStyle.h"
#include "Bico_STM8_Wire.h"

#define NUM_BYTE 1

void setup()
{
  Serial.begin(57600);
  I2C_Init(100000, (7<<1)|1, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 4);
}


void loop()
{
  I2C_GenerateSTART(ENABLE);
  uint8_t count = 0;
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
  delay_ms(1000);
  
  I2C_Send7bitAddress(8<<1, I2C_DIRECTION_RX);
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
  
  /* Disable Acknowledgement */
  //I2C_AcknowledgeConfig(I2C_ACK_NONE);

  /* Send STOP Condition */
  //I2C_GenerateSTOP(ENABLE);
  
  for(int i = NUM_BYTE; i > 0; i--)
  {
    if(i != 3)
    {
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_TRANSFERFINISHED\r\n");
      #endif
        if(count++ > WAITING_TIMEOUT)
        {
          break;
        }
      }
      Serial.write(I2C_ReceiveData());
    }
    
    if(i == 3)
    {
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_TRANSFERFINISHED3\r\n");
      #endif
        if(count++ > WAITING_TIMEOUT)
        {
          break;
        }
      }
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
      Serial.write(I2C_ReceiveData());
      i--;
      I2C_GenerateSTOP(ENABLE);
      Serial.write(I2C_ReceiveData());
      i--;
 
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_RXNOTEMPTY\r\n");
      #endif
        if(count++ > WAITING_TIMEOUT)
        {
          break;
        }
      }
      Serial.write(I2C_ReceiveData());
      i--;
      
    }
  }
  Wire.end();
  Wire.begin();
  Serial.println();

  delay_ms(3000);
}
