#include <string.h>

#include "eeprom.h"
#include "crc.h"

#define EEPROM_SIZE		0x400
#define EEPROM_BASE_0	(uint32_t)(FLASH_EEPROM_BASE)
#define EEPROM_BASE_1	(uint32_t)(FLASH_EEPROM_BASE + EEPROM_SIZE)

#define EEPROM_CONFIG_0	((Config_t *)EEPROM_BASE_0)
#define EEPROM_CONFIG_1	((Config_t *)EEPROM_BASE_1)

const Config_t Config_Default = {
	.Checksum = 0,
	.Address = EEPROM_BASE_1,
	.Size = sizeof(Config_t),
	.Version = 0,
// Set default to 5V
// 255		=  2.81V
// 183		=  3.31V
// 79-81	=  5.02V
//   0		= 10.55V
	.MAX5400_0 = 79,	.MAX5400_1 = 79,

	.MAX5380_0 = 0,		.MAX5380_1 = 0,
	.MAX16936_0 = 0,	.MAX16936_1 = 0,
	0
};

Config_t Config;
// const Config_t Config_0 __attribute__((at(EEPROM_BASE_0)));
// const Config_t Config_1 __attribute__((at(EEPROM_BASE_1)));

/*
 *
 *	Calculate checksum
 */
bool Config_Check(const Config_t * config)
{
	uint32_t data;
	uint16_t size;
	uint8_t * buffer = (uint8_t *)config;

	if (
		(config->Address == EEPROM_BASE_0 || config->Address == EEPROM_BASE_1)
	&&	(config->Size > 0)
	&&	(config->Size < EEPROM_SIZE)
		)
	{
		size = config->Size;

		__HAL_CRC_DR_RESET(&hcrc);

		while (size != 0)
		{
			data = 0;
			if (size >= 4)
			{
				data = *(uint32_t *)buffer;
				buffer += 4;
				size -= 4;
			}
			else
			{
				while (size != 0)
				{
					data <<= 8;
					data |= (uint32_t)(*buffer);
					buffer++;
					--size;
				}
			}
			hcrc.Instance->DR = data;
		}
		if (config->Checksum == hcrc.Instance->DR)
			return true;
	}
	return false;
}

void Config_Init(void)
{
	uint16_t size;
	const Config_t * current = EEPROM_CONFIG_0;

	if (Config_Check(EEPROM_CONFIG_0))
	{
		if (Config_Check(EEPROM_CONFIG_1)
		&& EEPROM_CONFIG_1->Version > EEPROM_CONFIG_0->Version
			)
			current = EEPROM_CONFIG_1;
	}
	else if (Config_Check(EEPROM_CONFIG_1))
		current = EEPROM_CONFIG_1;
	else
		current = &Config_Default;

	size = current->Size;
	if (size > sizeof(Config_t))
		size = sizeof(Config_t);

	if (size < sizeof(Config_t))
		memcpy(&Config, &Config_Default, sizeof(Config_t));

	memcpy(&Config, current, size);
}

bool Config_Save(void)
{

	return true;
}

void Config_Print(void)
{
	printf("CONFIG:\n");
	printf("MAX5400_0:%d\n", Config.MAX5400_0);
	printf("MAX5400_1:%d\n", Config.MAX5400_1);
	printf("MAX5380_0:%d\n", Config.MAX5380_0);
	printf("MAX5380_1:%d\n", Config.MAX5380_1);
	printf("MAX16936_0:%d\n", Config.MAX16936_0);
	printf("MAX16936_1:%d\n", Config.MAX16936_1);
}
