#include "Bico_STM8_Analog.h"
#include "Bico_STM8_Gpio.h"

uint8_t analogRead(uint8_t pin)
{
  uint8_t analog_channel;
  
  switch(pin)
  {
    case AIN2: {analog_channel = ADC1_CHANNEL_2; break;}
    case AIN3: {analog_channel = ADC1_CHANNEL_3; break;}
    case AIN4: {analog_channel = ADC1_CHANNEL_4; break;}
    case AIN5: {analog_channel = ADC1_CHANNEL_5; break;}
    case AIN6: {analog_channel = ADC1_CHANNEL_6; break;}
  }
  
  ADC1_DeInit();
  pinMode(pin, GPIO_MODE_IN_FL_NO_IT);
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, (ADC1_Channel_TypeDef)analog_channel, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, (ADC1_SchmittTrigg_TypeDef)analog_channel,\
            DISABLE);

  ADC1_StartConversion();
  for(unsigned long i = 0; i < 2000000; i++)
  {
    if(ADC1_GetFlagStatus(ADC1_FLAG_EOC))
    {
      break;
    }
  }
  
  uint8_t val8 = (uint8_t)(ADC1_GetConversionValue()/4);
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  return val8;
}