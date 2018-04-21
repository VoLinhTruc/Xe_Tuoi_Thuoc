#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

void setup()
{
  clkConfig();
  Serial.begin(115200);
  pinMode(PB5, OUTPUT);
  
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, \
            ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
}

void loop()
{
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
  
  Serial.write(val8);
  Serial.write('\n');
  Serial.write('\r');
}
