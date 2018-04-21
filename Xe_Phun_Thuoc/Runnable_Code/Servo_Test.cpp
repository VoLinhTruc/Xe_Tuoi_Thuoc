#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"



void setup()
{
  clkConfig();
  //PWMConfig(TIM2_PRESCALER_16, 20000);
  Serial.begin(115200);
  pinMode(PB5, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PD3, OUTPUT);
  pinMode(PD4, OUTPUT);
  
  bico_tim2_prescaler = TIM2_PRESCALER_16;
  bico_tim2_period = 20000;

  TIM2_TimeBaseInit(bico_tim2_prescaler, bico_tim2_period);
  TIM2_Cmd(ENABLE);
}

void loop()
{
  for(int i = 500; i < 2500; i++)
  {
    TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, i, TIM2_OCPOLARITY_HIGH);
    delay_ms(1);
  }
  
  for(int i = 2500; i > 500; i--)
  {
    TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, i, TIM2_OCPOLARITY_HIGH);
    delay_ms(1);
  }
  
}
