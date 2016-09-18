#ifndef __MAX1169_H__
#define __MAX1169_H__

#include <stdint.h>
#include "i2c.h"

#define MAX1169_DEV_0	0x60
#define MAX1169_DEV_1	0x70

#define MAX1169_NORMAL	0x01

#ifdef __cplusplus
	extern "C" {
#endif

void MAX1169_Init(uint8_t device);

#ifdef __cplusplus
	}
#endif
	 
#endif
