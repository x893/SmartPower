#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef __packed struct {
	uint32_t	Checksum;
	uint32_t	Address;
	uint16_t	Size;
	uint16_t	Version;
	uint8_t		MAX5400_0,	MAX5400_1;
	uint8_t		MAX5380_0,	MAX5380_1;
	uint8_t		MAX16936_0,	MAX16936_1;
	uint8_t		Unused;
} Config_t;

extern Config_t Config;

void Config_Init(void);
bool Config_Save(void);

#ifdef __cplusplus
	}
#endif

#endif
