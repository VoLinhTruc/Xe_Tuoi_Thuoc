#include "Bico_STM8_Gpio.h"

uint16_t decodePort(uint8_t pin)
{
	if(pin < LSB_PB)
		return GPIOA_BaseAddress;
		
	else if(IS_BETWEEN(B, C))
		return GPIOB_BaseAddress;
		
	else if(IS_BETWEEN(C, D))
		return GPIOC_BaseAddress;
		
	else if(IS_BETWEEN(D, E))
		return GPIOD_BaseAddress;
        
        else return 0;
}

uint8_t decodePin(uint8_t pin)
{
	if(pin < LSB_PB)
		return (GPIO_Pin_TypeDef)((uint8_t)1 << (pin - LSB_PA));
		
	else if(IS_BETWEEN(B, C))
		return (GPIO_Pin_TypeDef)((uint8_t)1 << (pin - LSB_PB));
		
	else if(IS_BETWEEN(C, D))
		return (GPIO_Pin_TypeDef)((uint8_t)1 << (pin - LSB_PC));
		
	else if(IS_BETWEEN(D, E))
		return (GPIO_Pin_TypeDef)((uint8_t)1 << (pin - LSB_PD));
        
        else return 0;
}

void pinMode(uint8_t pin, GPIO_Mode_TypeDef mode)
{
	GPIO_Init((GPIO_TypeDef *) decodePort(pin), (GPIO_Pin_TypeDef)decodePin(pin), mode);
}

void digitalWrite(uint8_t pin, bool val)
{
        if((pin == PD4) || (pin == PD3) || (pin == PA3))
        {
            TIM2_DeInit();
        }
  
	if(val)
		GPIO_WriteHigh((GPIO_TypeDef*) decodePort(pin), (GPIO_Pin_TypeDef)decodePin(pin));
	else
		GPIO_WriteLow((GPIO_TypeDef*) decodePort(pin), (GPIO_Pin_TypeDef)decodePin(pin));
}

void digitalWriteReverse(uint8_t pin)
{
  ((GPIO_TypeDef*)decodePort(pin))->ODR ^= (GPIO_Pin_TypeDef)decodePin(pin);
} 

bool digitalRead(uint8_t pin)
{
	return (bool)GPIO_ReadInputPin((GPIO_TypeDef *) decodePort(pin), (GPIO_Pin_TypeDef)decodePin(pin));
}


void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}
