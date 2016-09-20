#include "MAX16936.h"
#include "MAX5380.h"

//	https://datasheets.maximintegrated.com/en/ds/MAX16936-MAX16938.pdf

#include "stm32l1xx_hal.h"
#include "gpio.h"


static void MAX16936_Set(uint8_t device, GPIO_PinState state)
{
	if (PS_IsConnect(device))
	{
		if (device == 0)
			HAL_GPIO_WritePin(MAX16936_EN0_GPIO_Port, MAX16936_EN0_Pin, state);
		else if (device == 1)
			HAL_GPIO_WritePin(MAX16936_EN1_GPIO_Port, MAX16936_EN1_Pin, state);
	}
}

/****************************************************************
 *
 *	Enable MAX16936
 *
 */
void MAX16936_Enable(uint8_t device)
{
	MAX16936_Set(device, GPIO_PIN_SET);
}

/****************************************************************
 *
 *	Disable MAX16936
 *
 */
void MAX16936_Disable(uint8_t device)
{
	MAX16936_Set(device, GPIO_PIN_RESET);
}
