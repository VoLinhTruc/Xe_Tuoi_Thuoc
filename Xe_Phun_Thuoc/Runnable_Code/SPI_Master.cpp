#include "ArduinoStyle.h"

char c = 0;

void setup()
{
  Serial.begin(57600);
  
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, \
           SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, (uint8_t)0x07);
  SPI_Cmd(ENABLE);
  
  pinMode(PB5, OUTPUT);
  pinMode(PD3, OUTPUT);
}



void loop()
{
  digitalWriteReverse(PB5);
  delay_ms(500);
  
  digitalWrite(PD3, 0);
  SPI_SendData(c++);
  delay_us(100);
  Serial.write(SPI_ReceiveData());
  digitalWrite(PD3, 1);
}
