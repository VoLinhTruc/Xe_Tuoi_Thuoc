#include "Bico_STM8_Generic.h"

void delay_us(unsigned int x)
{
     while(x--)
     {
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
     }
}

void delay_ms(unsigned int x)
{
     while(x--)
     {
        delay_us(1000);
     }
}



void clkConfig()
{
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void enableGlobalInterrupt()
{
#ifndef INTERRUPT_WAS_ENABLED
#define INTERRUPT_WAS_ENABLED
  enableInterrupts();
#endif
}

void disableGlobalInterrupt()
{
#ifndef INTERRUPT_WAS_DISABLED
#define INTERRUPT_WAS_DISABLED
  disableInterrupts();
#endif
}