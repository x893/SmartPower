#include "MAX16936.h"

//	https://datasheets.maximintegrated.com/en/ds/MAX16936-MAX16938.pdf

#include "stm32l1xx_hal.h"
#include "gpio.h"

/****************************************************************
 *
 *	Enable MAX16936
 *
 */
void MAX16936_Enable(uint8_t device)
{
	if (device == 0)
	{
		HAL_GPIO_WritePin(MAX16936_EN0_GPIO_Port, MAX16936_EN0_Pin, GPIO_PIN_SET);
	}
	else if (device == 1)
	{
		HAL_GPIO_WritePin(MAX16936_EN1_GPIO_Port, MAX16936_EN1_Pin, GPIO_PIN_SET);
	}
}

/****************************************************************
 *
 *	Disable MAX16936
 *
 */
void MAX16936_Disable(uint8_t device)
{
	if (device == 0)
	{
		HAL_GPIO_WritePin(MAX16936_EN0_GPIO_Port, MAX16936_EN0_Pin, GPIO_PIN_RESET);
	}
	else if (device == 1)
	{
		HAL_GPIO_WritePin(MAX16936_EN1_GPIO_Port, MAX16936_EN1_Pin, GPIO_PIN_RESET);
	}
}
