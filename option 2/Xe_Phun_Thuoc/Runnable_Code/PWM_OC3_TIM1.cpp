#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

void setup()
{
  clkConfig();
  Serial.begin(115200);
  pinMode(PB5, OUTPUT);
  
  //CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 100, 0);
  //TIM1_ARRPreloadConfig(ENABLE);
  //TIM1_OC3PreloadConfig(ENABLE);
  TIM1_Cmd(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
}

void loop()
{
  uint8_t a = (uint8_t)((uint16_t)analogRead(AIN2)*100/255);
  Serial.write(a);
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, \
                TIM1_OUTPUTNSTATE_DISABLE, a, TIM1_OCPOLARITY_HIGH,\
                  TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
}
