#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

#pragma vector = 21  /* Symbol from I/O */ 
__interrupt void I2C_ISR(void)
{
  
}

void setup()
{
  Serial.begin(57600);
  
  I2C_Init(100000, 7, 
              I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
              I2C_ADDMODE_7BIT, 4);
  I2C_Cmd(ENABLE);
  I2C_GeneralCallCmd(ENABLE);
  //I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
}
uint8_t count = 0;
void loop()
{
  I2C_GenerateSTART(ENABLE);
  delay_ms(2000);
  I2C_Send7bitAddress((8<<1), I2C_DIRECTION_TX);
  while((I2C->SR1 & 2) == 0);
  while((I2C->SR3 & 1) == 0);
  delay_ms(1000);
  I2C_SendData(1);
  while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == 0);
  I2C_SendData(2);
  while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == 0);
  I2C_SendData(3);
  while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == 0);
  I2C_SendData(4);
  delay_ms(1000);
  I2C_GenerateSTOP(ENABLE);
  
  delay_ms(1000);
}
