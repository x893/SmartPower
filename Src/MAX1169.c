#include "MAX1169.h"
#include "MAX5380.h"
#include "i2c_board.h"

//	58.6ksps, 16-Bit, 2-Wire Serial ADC
//	https://datasheets.maximintegrated.com/en/ds/MAX1169.pdf

typedef __packed struct {
	uint16_t	Data;
	uint8_t		ACK;
} MAX1169_Data_t;

MAX1169_Data_t MAX1169_0_Data;
MAX1169_Data_t MAX1169_1_Data;

/****************************************************************
 *
 *	Read MAX1169
 *
 */
bool MAX1169_Read(uint8_t device)
{
	int timeout;
	GPIO_TypeDef * scl_port = SDA_GPIO_Port;
	uint16_t scl_pin = SDA_Pin;
	bool ack = false;
	MAX1169_Data_t * pData;

	pData->ACK = 0;
	if (PS_IsNotConnect(device))
		return ack;

	if (device == 0)
	{
		device = MAX1169_DEV_0;
		pData = &MAX1169_0_Data;
	}
	else if (device == 1)
	{
		device = MAX1169_DEV_1;
		pData = &MAX1169_1_Data;
	}
	else
		return ack;

	i2c_reset(SCL_GPIO_Port, SCL_Pin);
	i2c_start(SCL_GPIO_Port, SCL_Pin);

	if (i2c_write(SCL_GPIO_Port, SCL_Pin, device))
	{
		SDA_HIGH();
		SCL_HIGH();

		// Wait for clock stretch
		timeout = 50;

		while (SCL_READ() == GPIO_PIN_RESET && timeout != 0)
		{
			timeout--;
			DELAY_4();
		}

		if (SCL_READ() != GPIO_PIN_RESET)
		{
			i2c_read(SCL_GPIO_Port, SCL_Pin, (uint8_t *)&pData->Data, sizeof(pData->Data));
			pData->ACK = 1;
			ack = true;
		}
	}

	i2c_stop(SCL_GPIO_Port, SCL_Pin);

	return ack;
}

/****************************************************************
 *
 *	Initialize MAX1169
 *
 */
void MAX1169_Init(void)
{
	if (MAX1169_Read(0))
	{
		// MAX1169 presets
	}

	if (MAX1169_Read(1))
	{
		// MAX1169 presets
	}
}
