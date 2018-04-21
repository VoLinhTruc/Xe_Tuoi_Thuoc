#include "Bico_STM8_Servo.h"

Servo::Servo()
{
   bico_tim2_prescaler = TIM2_PRESCALER_16;
   bico_tim2_period = REFRESH_INTERVAL;
}

uint8_t Servo::attach(int pin)
{
	this->pin = pin;
        
        if((pin == PD4) || (pin == PD3) ||(pin == PA4))
          return 1;
        else
          return 0;
}

uint8_t Servo::attach(int pin, int min, int max)
{
	this->pin = pin;
	this->min = min;
	this->max = max;
        
        if((pin == PD4) || (pin == PD3) ||(pin == PA4))
          return 1;
        else
          return 0;
}

void Servo::detach()
{

}

void Servo::write(uint8_t value)
{
	TIM2_TimeBaseInit(bico_tim2_prescaler, bico_tim2_period);
	TIM2_Cmd(ENABLE);

	value = (value > 180) ? 180:value;

	uint16_t val = value*((MAX_PULSE_WIDTH - MIN_PULSE_WIDTH)/180) + MIN_PULSE_WIDTH;
	switch (pin)
	{
		case PD4: TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, val, TIM2_OCPOLARITY_HIGH);
			break;
		case PD3: TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, val, TIM2_OCPOLARITY_HIGH);
			break;
		case PA3: TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, val, TIM2_OCPOLARITY_HIGH);
			break;
	}
}

