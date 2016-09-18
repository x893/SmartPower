#ifndef __MAX5400_H__
#define __MAX5400_H__

#include <stdint.h>
#include "i2c_board.h"

#ifdef __cplusplus
	extern "C" {
#endif

void MAX5400_Set(uint8_t device, uint8_t value);

#ifdef __cplusplus
	}
#endif
	 
#endif
