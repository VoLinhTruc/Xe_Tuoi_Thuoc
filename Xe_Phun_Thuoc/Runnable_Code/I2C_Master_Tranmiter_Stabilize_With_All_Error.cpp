#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"
#include "Bico_STM8_Wire.h"

#define TIMER 50 //time-out cho moi hanh dong cua I2C
int timer = TIMER; // in us

#define LENGTH 2 // do dai du lieu can test
int length = LENGTH;


uint8_t reset = 0; // neu =1 thi reset I2C module, neu =0 thi thoi

void setup()
{
  Serial.begin(57600);
  enableGlobalInterrupt();
  I2C_Init(I2C_FREQ, (8<<1), I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, I2C_INPUT_FREQ);
}


void loop()
{
  //Neu khong co tin hieu need to reset timer
  if(reset == 0)
  {
    //START generation
    I2C_GenerateSTART(ENABLE);
    int count = 0;
    //wait until "count" reach "TIMER" value
    while(I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
    {
      //Moniting hanh dong hien tai cua I2C
      Serial.print("I2C_GenerateSTART\r\n");
      if(count++ > TIMER)
      {
        //neu "count" vuot qua "TIMER" (het timeout cho 1 hanh dong cua I2C) thi reset=1
        reset = 1;
        break;
      }
      delay_us(1);
    }

    //Send address and direction
    I2C_Send7bitAddress(8<<1, I2C_DIRECTION_TX);
    count = 0;
    while(I2C_GetLastEvent() != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
    {
      //Moniting hanh dong hien tai cua I2C
      Serial.print("I2C_Send7bitAddress\r\n");
      if(count++ > TIMER)
      {
        //neu "count" vuot qua "TIMER" (het timeout cho 1 hanh dong cua I2C) thi reset=1
        reset = 1;
        break;
      }
      delay_us(1);
    }
    
    
    for(int i = 0; i < length; i++)
    {
      //Sent Data
      I2C_SendData(51);
      count = 0;
      while(I2C_GetLastEvent() != I2C_EVENT_MASTER_BYTE_TRANSMITTED)
      {
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_SendData\r\n");
        if(count++ > TIMER)
        {
          //neu "count" vuot qua "TIMER" (het timeout cho 1 hanh dong cua I2C) thi reset=1
          reset = 1;
          break;
        }
        delay_us(1);
      }
    }
    
    //Generating Stop after send all data
    I2C_GenerateSTOP(ENABLE);
  }

  //Neu co loi xay ra thi reset=1, vi vay ta can reset lai module I2C
  if(reset == 1)
  {
    //xoa BUSY bit neu duong truyen ban, khong the xoa truc tiep bang thanh ghi (tham khao RM0016 Reference manual trang 306/260)
    I2C_SoftwareResetCmd(ENABLE);
    
    //reset cac thanh ghi I2C, ngoai tru 4 thanh ghi ben duoi 
    I2C_DeInit();
    
    //Reset cac thanh ghi con lai cua module I2C
    I2C->SR1 = 0;
    I2C->SR2= 0;
    I2C->SR3 = 0;
    I2C->DR = 0;
    
    //monitoring I2C register
    Serial.write(I2C->CR1);
    Serial.write(I2C->CR2);
    Serial.write(I2C->FREQR);
    Serial.write(I2C->OARL);
    Serial.write(I2C->OARH);
    Serial.write(I2C->DR);
    Serial.write(I2C->SR1);
    Serial.write(I2C->SR2);
    Serial.write(I2C->SR3);
    Serial.write(I2C->ITR);
    Serial.write(I2C->CCRL);
    Serial.write(I2C->CCRH);
    Serial.write(I2C->TRISER);
    
    //Init I2C after reset all I2C registers
    I2C_Init(I2C_FREQ, (8<<1), I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, I2C_INPUT_FREQ);
    //after reset I2C module, make reset=0
    reset = 0;
  }
  
  delay_ms(2000);
}
