//Thu vien nay con qua so sai, chua tuong xung voi Arduino, can cai tao lai

#ifndef BICO_STM8_SPI_H
#define BICO_STM8_SPI_H

#include "stm8s.h"

#ifdef SPI
#undef SPI
#endif

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define MOSI PC6
#define MISO PC7
#define SCK PC5

extern void delay_us(unsigned int x);

class SPISettings 
{
public:
	SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
	SPISettings(); //defautl see at https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/libraries/SPI/src/SPI.h

	SPI_FirstBit_TypeDef FirstBit;
	SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler;
	SPI_ClockPolarity_TypeDef ClockPolarity;
	SPI_ClockPhase_TypeDef ClockPhase;
};

class SPIClass {
public:
	SPIClass();
	void begin();
	void beginTransaction(SPISettings settings);
    void beginTransaction();
	uint8_t transfer(uint8_t data);
	//uint16_t transfer16(uint16_t data);
	//void transfer(void *buf, size_t count);
	void endTransaction(void);
	void end();

	//void setBitOrder(uint8_t bitOrder);
	//void setDataMode(uint8_t dataMode);
	//void setClockDivider(uint8_t clockDiv);

};

extern SPIClass SPI;

#endif