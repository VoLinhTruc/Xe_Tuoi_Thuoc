#ifndef BICO_STM8_EEPROM_H
#define BICO_STM8_EEPROM_H


#include "stm8s.h"

#ifdef EEPROM
#undef EEPROM
#endif

#define BEGIN_ADDRESS 0x4000
#define END_ADDRESS 0x427F
#define LITMIT_ADDRESS 0x0280

class Data_EEPROM
{
public:
	Data_EEPROM();
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
	void update(uint16_t address, uint8_t value);
	//void get(uint16_t address, auto obj);
	//void put(uint16_t address, auto obj);

};

extern Data_EEPROM EEPROM;

#endif