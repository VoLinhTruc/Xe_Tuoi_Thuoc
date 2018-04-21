#ifndef BICO_STM8_GPIO_H
#define BICO_STM8_GPIO_H

#include "stm8s.h"

#ifndef HIGH
  #define HIGH 1
#endif

#ifndef LOW
  #define LOW 0
#endif

#ifndef CHANGE
  #define CHANGE 2
#endif

#ifndef RISING
  #define RISING  3
#endif

#ifndef FALLING
  #define FALLING 4
#endif

#ifndef LSBFIRST
#define LSBFIRST 0x80 //0x80 cho phu hop voi thu vien SPI
#endif

#ifndef MSBFIRST
#define MSBFIRST 0x00 //0x00 cho phu hop voi thu vien SPI
#endif

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 8
#define PB1 9
#define PB2 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15

#define PC0 16
#define PC1 17
#define PC2 18
#define PC3 19
#define PC4 20
#define PC5 21
#define PC6 22
#define PC7 23

#define PD0 24
#define PD1 25
#define PD2 26
#define PD3 27
#define PD4 28
#define PD5 29
#define PD6 30
#define PD7 31

#define PE0 32
#define PE1 33
#define PE2 34
#define PE3 35
#define PE4 36
#define PE5 37
#define PE6 38
#define PE7 39

#define PF0 40 
#define PF1 41 
#define PF2 42 
#define PF3 43 
#define PF4 44 
#define PF5 45 
#define PF6 46 
#define PF7 47 

#define PG0 48 
#define PG1 49 
#define PG2 50 
#define PG3 51 
#define PG4 52 
#define PG5 53 
#define PG6 54 
#define PG7 55 

#define PH0 56 
#define PH1 57 
#define PH2 58 
#define PH3 59 
#define PH4 60 
#define PH5 61 
#define PH6 62 
#define PH7 63 

#define PI0 64 
#define PI1 65 
#define PI2 66 
#define PI3 67 
#define PI4 68 
#define PI5 69 
#define PI6 70 
#define PI7 71

#define LSB_PA 0 
#define LSB_PB 8 
#define LSB_PC 16 
#define LSB_PD 24 
#define LSB_PE 32 
#define LSB_PF 40 
#define LSB_PG 48 
#define LSB_PH 56 
#define LSB_PI 64 


#define IS_BETWEEN(p1, p2) (pin >= LSB_P##p1)&&(pin < LSB_P##p2)

#define BicoPort8 uint8_t
#define BicoPort16 uint16_t

#define OUTPUT GPIO_MODE_OUT_PP_LOW_FAST
#define INPUT GPIO_MODE_IN_FL_NO_IT
#define INPUT_PULLUP GPIO_MODE_IN_PU_NO_IT



uint16_t decodePort(uint8_t pin);
uint8_t decodePin(uint8_t pin);

void pinMode(uint8_t pin, GPIO_Mode_TypeDef mode);
void digitalWrite(uint8_t pin, bool val);
void digitalWriteReverse(uint8_t pin);
bool digitalRead(uint8_t pin);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

#endif