#include "Bico_STM8_Eeprom.h"

Data_EEPROM::Data_EEPROM()
{
	
}

uint8_t Data_EEPROM::read(uint16_t address)
{
	if(address < LITMIT_ADDRESS)
	{
		return FLASH_ReadByte(BEGIN_ADDRESS +  address);
	}
	else return 0xFF;
}

void Data_EEPROM::write(uint16_t address, uint8_t value)
{
	if(address < LITMIT_ADDRESS)
	{
		FLASH_ProgramByte(BEGIN_ADDRESS + address, value);
	}
}

void Data_EEPROM::update(uint16_t address, uint8_t value)
{
	if(FLASH_ReadByte(BEGIN_ADDRESS +  address) != value)
	{
		write(address, value);
	}
}

Data_EEPROM EEPROM;