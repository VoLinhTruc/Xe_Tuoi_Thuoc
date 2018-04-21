#ifndef BICO_STM8_ANALOG_H
#define BICO_STM8_ANALOG_H

#define AIN2 PC4
#define AIN3 PD2
#define AIN4 PD3
#define AIN5 PD5
#define AIN6 PD6

#include "stm8s.h"

uint8_t analogRead(uint8_t pin);

#endif