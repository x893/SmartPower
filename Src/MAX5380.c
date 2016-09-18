#include "MAX5380.h"

//	https://datasheets.maximintegrated.com/en/ds/MAX5380-MAX5382.pdf

/****************************************************************
 *
 *	Send data to MAX5380
 *
 */
static uint8_t MAX5380_Send(GPIO_TypeDef * port, uint16_t pin, uint8_t data)
{
	uint8_t mask = 0x80;
	while (mask != 0)
	{	// Set DIN (MSB)
		HAL_GPIO_WritePin(
			SDA_GPIO_Port, SDA_Pin,
			(data & mask) ? GPIO_PIN_SET : GPIO_PIN_RESET
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

	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	if (HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin) == GPIO_PIN_RESET)
		mask = 1;

	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	return mask;
}

/****************************************************************
 *
 *	Set value MAX5380
 *	Currectly separate SCLK for MAX5380
 *	TODO: Change to general SCL signal
 */
uint8_t MAX5380_Set(uint8_t device, uint8_t value)
{
	GPIO_TypeDef * scl_port;
	uint16_t scl_pin;
	uint8_t result = 0;

	if (device == 0)
	{
		scl_port = MAX5380_SCL0_GPIO_Port;
		scl_pin = MAX5380_SCL0_Pin;
	}
	else if (device == 1)
	{
		scl_port = MAX5380_SCL1_GPIO_Port;
		scl_pin = MAX5380_SCL1_Pin;
	}
	else
		return result;

	SDA_GPIO_Set();

	// Start contintion
	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET);
	if (MAX5380_Send(scl_port, scl_pin, device))
	{
		// Device respone
		if (MAX5380_Send(scl_port, scl_pin, value))
		{
			result = 1;
		}
	}

	// Stop contintion
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);

	SDA_I2C_Set();

	return result;
}

/****************************************************************
 *
 *	Shutdown MAX5380
 *
 */
uint8_t MAX5380_Shutdown(uint8_t device)
{
	return MAX5380_Set(device | MAX5380_SHDN, 0);
}
