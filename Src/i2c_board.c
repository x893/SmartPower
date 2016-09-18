#include "i2c_board.h"

const GPIO_InitTypeDef SDA_GPIO = {
	SDA_Pin,
	GPIO_MODE_OUTPUT_OD,
	GPIO_NOPULL,
	GPIO_SPEED_FREQ_VERY_HIGH
};

const GPIO_InitTypeDef SDA_I2C = {
	SDA_Pin,
	GPIO_MODE_AF_OD,
	GPIO_NOPULL,
	GPIO_SPEED_FREQ_VERY_HIGH,
	GPIO_AF4_I2C2
};

void SDA_GPIO_Set(void)
{
	__HAL_I2C_DISABLE(&hi2c2);

	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
	HAL_GPIO_Init(SDA_GPIO_Port, (GPIO_InitTypeDef *)&SDA_GPIO);
	
}

void SDA_I2C_Set(void)
{
    HAL_GPIO_Init(SDA_GPIO_Port, (GPIO_InitTypeDef *)&SDA_I2C);

	__HAL_I2C_ENABLE(&hi2c2);
}

