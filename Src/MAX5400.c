#include "MAX5400.h"
#include "MAX5380.h"

//	256-Tap SOT-PoT, Low-Drift Digital Potentiometers in SOT23
//
//	https://datasheets.maximintegrated.com/en/ds/MAX5400-MAX5401.pdf

#include "stm32l1xx_hal.h"
#include "gpio.h"

/****************************************************************
 *
 *	Set value MAX5400
 *
 */
void MAX5400_Set(uint8_t device, uint8_t value)
{
	GPIO_TypeDef * port;
	uint16_t pin;
	uint8_t mask = 0x80;

	if (PS_IsNotConnect(device))
		return;

	if (device == 0)
	{
		port = MAX5400_SCL0_GPIO_Port;
		pin = MAX5400_SCL0_Pin;
	}
	else if (device == 1)
	{
		port = MAX5400_SCL1_GPIO_Port;
		pin = MAX5400_SCL1_Pin;
	}
	else
		return;

	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	if (device == 0)
		HAL_GPIO_WritePin(MAX5400_CS0_GPIO_Port, MAX5400_CS0_Pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(MAX5400_CS1_GPIO_Port, MAX5400_CS1_Pin, GPIO_PIN_RESET);

	while (mask != 0)
	{	// Set DIN (MSB)
		HAL_GPIO_WritePin(
			SDA_GPIO_Port, SDA_Pin,
			(value & mask) ? GPIO_PIN_SET : GPIO_PIN_RESET
		);

		// Pulse on SCLK
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

		mask >>= 1;
	}

	if (device == 0)
		HAL_GPIO_WritePin(MAX5400_CS0_GPIO_Port, MAX5400_CS0_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(MAX5400_CS1_GPIO_Port, MAX5400_CS1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
}
