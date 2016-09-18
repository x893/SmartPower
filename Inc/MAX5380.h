#ifndef __MAX5400_H__
#define __MAX5400_H__

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

uint8_t MAX5380_Set(uint8_t device, uint8_t value);
uint8_t MAX5380_Shutdown(uint8_t device);
		
#ifdef __cplusplus
	}
#endif
	 
#endif
