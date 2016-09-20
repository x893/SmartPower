#ifndef __I2C_BOARD_H__
#define __I2C_BOARD_H__

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

#ifdef __cplusplus
	extern "C" {
#endif

void i2c_init(void);
void i2c_reset(void);		
void i2c_start(GPIO_TypeDef * scl_port, uint16_t scl_pin);
void i2c_stop(GPIO_TypeDef * scl_port, uint16_t scl_pin);
bool i2c_write(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t data);
void i2c_read(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t * data, uint16_t count);

#ifdef __cplusplus
	}
#endif
	 
#endif
