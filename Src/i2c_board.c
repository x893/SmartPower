#include "i2c_board.h"

#define SCL_HIGH()	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET)
#define SCL_LOW()	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET)

#define SDA_HIGH()	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SDA_LOW()	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define SDA_READ()	HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

#define DELAY_4()	do { __NOP();__NOP();__NOP();__NOP(); } while (0)

void i2c_start(GPIO_TypeDef * scl_port, uint16_t scl_pin)
{
	SDA_HIGH();
	DELAY_4();
	SCL_HIGH();
	DELAY_4();
	SDA_LOW();
	DELAY_4();
	SCL_LOW();
}

void i2c_stop(GPIO_TypeDef * scl_port, uint16_t scl_pin)
{
	SCL_LOW();
	DELAY_4();
	SDA_LOW();
	DELAY_4();
	SCL_HIGH();
	DELAY_4();
	SDA_HIGH();
}

/****************************************************************
 *
 *	Read data from I2C
 *
 */
void i2c_read(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t * pData, uint16_t count)
{
	uint8_t mask;
	uint8_t data;

	while (count != 0)
	{
		SDA_HIGH();

		data = 0;
		mask = 0x80;

		while (mask != 0)
		{
			DELAY_4();
			SCL_HIGH();
			DELAY_4();
			DELAY_4();

			if (SDA_READ() == GPIO_PIN_SET)
				data |= mask;

			SCL_LOW();
			mask >>= 1;
		}

		*pData++ = data;

		count--;
		if (count != 0)
			SDA_LOW();

		DELAY_4();
		SCL_HIGH();
		DELAY_4();
		SCL_LOW();
	}
}

/****************************************************************
 *
 *	Write data to I2C
 *
 */
bool i2c_write(GPIO_TypeDef * scl_port, uint16_t scl_pin, uint8_t data)
{
	uint8_t mask = 0x80;
	bool ack = false;

	while (mask != 0)
	{	// Set DIN (MSB)
		HAL_GPIO_WritePin(
			SDA_GPIO_Port, SDA_Pin,
			(data & mask) ? GPIO_PIN_SET : GPIO_PIN_RESET
		);

		// Pulse on SCLK
		DELAY_4();
		SCL_HIGH();
		DELAY_4();
		DELAY_4();
		SCL_LOW();

		mask >>= 1;
	}

	SDA_HIGH();
	DELAY_4();
	SCL_HIGH();
	DELAY_4();

	if (SDA_READ() == GPIO_PIN_RESET)
		ack = true;

	SCL_LOW();
	DELAY_4();
	SDA_LOW();

	return ack;
}

static void SCL_ALL_LOW(void)
{
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MAX5380_SCL0_GPIO_Port, MAX5380_SCL0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MAX5380_SCL1_GPIO_Port, MAX5380_SCL1_Pin, GPIO_PIN_RESET);
}

static void SCL_ALL_HIGH(void)
{
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MAX5380_SCL0_GPIO_Port, MAX5380_SCL0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MAX5380_SCL1_GPIO_Port, MAX5380_SCL1_Pin, GPIO_PIN_SET);
}

void i2c_init(void)
{
	int retry_count;
	int retry_count_clk;

	for (retry_count = 80; retry_count != 0; retry_count--)
	{
		DELAY_4();
		SCL_ALL_LOW();
		DELAY_4();
		SCL_ALL_HIGH();
		DELAY_4();
	}

	retry_count_clk = 10;
	while (SDA_READ() == GPIO_PIN_RESET && retry_count_clk != 0)
	{
		retry_count_clk--;
		DELAY_4();
		// Set clock high and wait for any clock stretching to finish.
		SCL_ALL_HIGH();
		DELAY_4();
		DELAY_4();
		SCL_ALL_LOW();
		DELAY_4();
		SCL_ALL_HIGH();
	}

	i2c_reset();

	retry_count = 10;
	while (SDA_READ() == GPIO_PIN_RESET && retry_count != 0)
	{
		retry_count--;
		DELAY_4();
	}

	if (retry_count == 0)
	{
		// i2c failure
	}
}

void i2c_reset(void)
{
	DELAY_4();
	SDA_LOW();
	DELAY_4();
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
	DELAY_4();
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
	DELAY_4();
	SDA_HIGH();
	DELAY_4();
}
