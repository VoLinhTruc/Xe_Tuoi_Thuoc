#ifndef BICO_STM8_EXIT_H
#define BICO_STM8_EXIT_H

#include "stm8s.h"
#include "Bico_STM8_Generic.h"

extern void enableGlobalInterrupt();

extern void (*EXIT_C7)();
extern void (*EXIT_D1)();

extern EXTI_Port_TypeDef digitalPinToInterrupt(uint8_t pin);
extern void attachInterrupt(EXTI_Port_TypeDef port, void (*ISR)(), uint8_t in_mode);

#endif