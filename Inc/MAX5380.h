#ifndef __MAX5380_H__
#define __MAX5380_H__

#include <stdint.h>
#include <stdbool.h>
#include "i2c_board.h"

#define MAX5380L		0x60
#define MAX5380M		0x62
#define MAX5380N		0x64
#define MAX5380P		0x66

#define MAX5380_SHDN	0x01

#ifdef __cplusplus
	extern "C" {
#endif

bool PS_IsConnect(uint8_t device);
bool PS_IsNotConnect(uint8_t device);

void MAX5380_Init(void);
bool MAX5380_Set(uint8_t device, uint8_t value);
bool MAX5380_Shutdown(uint8_t device);
		
#ifdef __cplusplus
	}
#endif
	 
#endif
