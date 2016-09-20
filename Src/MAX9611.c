#include "MAX9611.h"
#include "MAX5380.h"

//	High-Side, Current-Sense Amplifiers with 12-Bit ADC and Op Amp/Comparator
//
//	https://datasheets.maximintegrated.com/en/ds/MAX9611-MAX9612.pdf

#include "stm32l1xx_hal.h"
#include "gpio.h"

typedef __packed struct {
	uint16_t	CSA;
	uint16_t	RS;
	uint16_t	OUT;
	uint16_t	SET;
	uint16_t	TEMP;
	uint16_t	CR;
} MAX9611_Data_t;

typedef __packed struct {
	MAX9611_Data_t	Data;
		uint8_t		ACK;
} MAX9611_DataEx_t;

MAX9611_DataEx_t MAX9611_0_Data;
MAX9611_DataEx_t MAX9611_1_Data;

/****************************************************************
 *
 *	Write to register MAX9611
 *
 */
bool MAX9611_Write(uint8_t device, uint8_t address, uint8_t data)
{
	bool ack = false;

	if (device == 0)
		device = MAX9611_DEV_11;
	else if (device == 1)
		device = MAX9611_DEV_10;
	else
		return ack;

	i2c_reset(SCL_GPIO_Port, SCL_Pin);
	i2c_start(SCL_GPIO_Port, SCL_Pin);

	if (i2c_write(SCL_GPIO_Port, SCL_Pin, device)
	&&	i2c_write(SCL_GPIO_Port, SCL_Pin, address)
	&&	i2c_write(SCL_GPIO_Port, SCL_Pin, data)
		)
		ack = true;

	i2c_stop(SCL_GPIO_Port, SCL_Pin);

	return ack;
}

/****************************************************************
 *
 *	Convert registers MAX9611 to uint16_t
 *	+0: M7 M6 M5 M4 M3 M2 M1 M0
 *	
 */
__STATIC_INLINE void MAX9611_Convert(__packed uint16_t * pData)
{
	uint16_t data = *pData;
	*pData = (((data & 0xFF) << 4) | (data >> 12));
}

/****************************************************************
 *
 *	Read registers MAX9611
 *
 */
bool MAX9611_Read(uint8_t device)
{
	bool ack = false;
	MAX9611_DataEx_t * pData;

	pData->ACK = 0;
	
	if (PS_IsConnect(device))
		return ack;

	if (device == 0)
	{
		device = MAX9611_DEV_11;
		pData = &MAX9611_0_Data;
	}
	else if (device == 1)
	{
		device = MAX9611_DEV_10;
		pData = &MAX9611_1_Data;
	}
	else
		return ack;

	i2c_reset(SCL_GPIO_Port, SCL_Pin);
	i2c_start(SCL_GPIO_Port, SCL_Pin);

	if (i2c_write(SCL_GPIO_Port, SCL_Pin, device)
	&&	i2c_write(SCL_GPIO_Port, SCL_Pin, 0)
		)
	{
		i2c_start(SCL_GPIO_Port, SCL_Pin);
		if (i2c_write(SCL_GPIO_Port, SCL_Pin, device | MAX9611_READ))
		{
			i2c_read(SCL_GPIO_Port, SCL_Pin, (uint8_t *)&pData->Data, sizeof(MAX9611_Data_t));

			pData->ACK = 1;

			MAX9611_Convert(&pData->Data.CSA);
			MAX9611_Convert(&pData->Data.RS);
			MAX9611_Convert(&pData->Data.OUT);
			MAX9611_Convert(&pData->Data.SET);
			MAX9611_Convert(&pData->Data.TEMP);

			ack = true;
		}
	}

	i2c_stop(SCL_GPIO_Port, SCL_Pin);
	return ack;
}

/****************************************************************
 *
 *	Initialize MAX9611
 *	Remark:	For board 0 jumpers JP4 off, JP5 off (A1, A0 = 11)
 *			For board 1 jumpers JP4 off, JP5 on  (A1, A0 = 10)
 */
void MAX9611_Init(void)
{
	if (MAX9611_Write(0, MAX9611_CR1, MAX9611_CR1_MUX)
	&&	MAX9611_Write(0, MAX9611_CR2, 0)
		)
	{
		HAL_Delay(15);
		MAX9611_Read(0);
	}

	if (MAX9611_Write(1, MAX9611_CR1, MAX9611_CR1_MUX)
	&&	MAX9611_Write(1, MAX9611_CR2, 0)
		)
	{
		HAL_Delay(15);
		MAX9611_Read(1);
	}
}
