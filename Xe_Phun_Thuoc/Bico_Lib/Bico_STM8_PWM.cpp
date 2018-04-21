#include "Bico_STM8_PWM.h"
#include "Bico_STM8_Gpio.h"

TIM2_Prescaler_TypeDef bico_tim2_prescaler = TIM2_PRESCALER_16;
uint16_t bico_tim2_period = 2000;

void PWMConfig(TIM2_Prescaler_TypeDef prescaler, uint16_t period)
{
	bico_tim2_prescaler = prescaler;
	bico_tim2_period = period;
}

void analogWrite(uint8_t pin, uint8_t val)
{
	TIM2_TimeBaseInit(bico_tim2_prescaler, bico_tim2_period);
  	TIM2_Cmd(ENABLE);
	switch (pin)
	{
		case PD4: TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, (uint16_t)((uint32_t)val*(uint32_t)bico_tim2_period/255), TIM2_OCPOLARITY_HIGH);
			break;
		case PD3: TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, (uint16_t)((uint32_t)val*(uint32_t)bico_tim2_period/255), TIM2_OCPOLARITY_HIGH);
			break;
		case PA3: TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, (uint16_t)((uint32_t)val*(uint32_t)bico_tim2_period/255), TIM2_OCPOLARITY_HIGH);
			break;
	}
}