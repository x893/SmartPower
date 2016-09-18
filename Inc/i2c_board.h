#ifndef __I2C_BOARD_H__
#define __I2C_BOARD_H__

#include <stdint.h>
#include "i2c.h"

#ifdef __cplusplus
	extern "C" {
#endif

void SDA_GPIO_Set(void);
void SDA_I2C_Set(void);

#ifdef __cplusplus
	}
#endif
	 
#endif
