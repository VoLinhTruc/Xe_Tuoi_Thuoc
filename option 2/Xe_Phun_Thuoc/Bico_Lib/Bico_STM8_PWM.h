/*
This PWM library just support on pin PD4(TIM2_OC1), PD3(TIM2_OC2), PA3(TIM2_OC3)
*/

#ifndef BICO_STM8_PWM_H
#define BICO_STM8_PWM_H

#include "stm8s.h"

extern TIM2_Prescaler_TypeDef bico_tim2_prescaler;
extern uint16_t bico_tim2_period;

extern void PWMConfig(TIM2_Prescaler_TypeDef prescaler, uint16_t period);

extern void analogWrite(uint8_t pin, uint8_t val);

#endif