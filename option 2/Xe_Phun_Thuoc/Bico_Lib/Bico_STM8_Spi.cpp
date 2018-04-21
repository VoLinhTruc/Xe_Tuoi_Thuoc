#include "Bico_STM8_Spi.h"

SPISettings::SPISettings()
{
	FirstBit = SPI_FIRSTBIT_MSB;

	uint8_t cpu_clk = (uint8_t)(CLK_GetClockFreq()/(uint32_t)1000000); //in MHz unit
	switch(cpu_clk)
	{
		case 16:
		{
			BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
			break;
		}

		case 8:
		{
			BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
			break;
		}
	}

	ClockPolarity = SPI_CLOCKPOLARITY_LOW;

	ClockPhase = SPI_CLOCKPHASE_2EDGE;
}

SPISettings::SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
	uint8_t cpu_clk = (uint8_t)(CLK_GetClockFreq()/(uint32_t)1000000); //in MHz unit
	switch(cpu_clk)
	{
		case 16:
		{
			switch(clock)
			{
				case (uint32_t)8000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
					break;
				}

				case (uint32_t)4000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
					break;
				}

				case (uint32_t)2000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
					break;
				}

				case (uint32_t)1000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
					break;
				}
			}
			break;
		}

		case 8:
		{
			switch(clock)
			{
				case (uint32_t)4000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
					break;
				}

				case (uint32_t)2000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
					break;
				}

				case (uint32_t)1000000:
				{
					BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
					break;
				}
			}
			break;
		}
	}

	switch(bitOrder)
	{
		case LSBFIRST:
		{
			FirstBit = SPI_FIRSTBIT_LSB;
			break;
		}

		case MSBFIRST:
		{
			FirstBit = SPI_FIRSTBIT_MSB;
			break;
		}
	}

	switch(dataMode)
	{
		case SPI_MODE0:
		{
			ClockPolarity = SPI_CLOCKPOLARITY_LOW;
			ClockPhase = SPI_CLOCKPHASE_1EDGE;
			break;
		}

		case SPI_MODE1:
		{
			ClockPolarity = SPI_CLOCKPOLARITY_LOW;
			ClockPhase = SPI_CLOCKPHASE_2EDGE;
			break;
		}

		case SPI_MODE2:
		{
			ClockPolarity = SPI_CLOCKPOLARITY_HIGH;
			ClockPhase = SPI_CLOCKPHASE_1EDGE;
			break;
		}

		case SPI_MODE3:
		{
			ClockPolarity = SPI_CLOCKPOLARITY_HIGH;
			ClockPhase = SPI_CLOCKPHASE_2EDGE;
			break;
		}
	}

}

//------------------------------------------------------------

SPIClass::SPIClass()
{

}

void SPIClass::begin()
{

}

void SPIClass::beginTransaction(SPISettings settings)
{
	SPI_Init(settings.FirstBit, settings.BaudRatePrescaler, SPI_MODE_MASTER, settings.ClockPolarity, \
           settings.ClockPhase, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, (uint8_t)0x07);
	SPI_Cmd(ENABLE);
}

void SPIClass::beginTransaction()
{
  beginTransaction(SPISettings());
}

uint8_t SPIClass::transfer(uint8_t data)
{
	SPI_SendData(data);
	delay_us(100);
	return SPI_ReceiveData();
}

void SPIClass::endTransaction()
{
	SPI_DeInit();
	SPI_Cmd(DISABLE);
}

void SPIClass::end()
{
	
}

SPIClass SPI;