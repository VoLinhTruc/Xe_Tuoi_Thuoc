#ifndef BICO_STM8_GENERIC_H
#define BICO_STM8_GENERIC_H

#define ARDUINO 100

#define delay delay_ms
#define delayMicroseconds delay_us

#include "stm8s.h"
#include "Bico_STM8_Gpio.h"
#include "Bico_STM8_Serial.h"
#include "Bico_STM8_Analog.h"
#include "Bico_STM8_PWM.h"
#include "Bico_STM8_Eeprom.h"
#include "Bico_STM8_Exti.h"

void delay_us(unsigned int x);
void delay_ms(unsigned int x);

void clkConfig();
void enableGlobalInterrupt();


#endif