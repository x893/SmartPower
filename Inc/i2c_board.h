#ifndef __I2C_BOARD_H__
#define __I2C_BOARD_H__

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define SCL_HIGH()	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET)
#define SCL_LOW()	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET)
#define SCL_READ()	HAL_GPIO_ReadPin(scl_port, scl_pin)

#define SDA_HIGH()	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SDA_LOW()	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define SDA_READ()	HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

#define DELAY_4()	do { __NOP();__NOP();__NOP();__NOP(); } while (0)

void i2c_init(void);
void i2c_reset(GPIO_TypeDef * scl_port, uint16_t scl_pin);
void i2c_start(GPIO_TypeDef * scl_port, uint16_t scl_pin);
void i2c_stop(GPIO_TypeDef * scl_port, uint16_t scl_pin);
bool i2c_write(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t data);
void i2c_read(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t * data, uint16_t count);

#ifdef __cplusplus
	}
#endif
	 
#endif
