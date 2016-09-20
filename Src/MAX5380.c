#include "MAX5380.h"
#include "i2c_board.h"

//	 Low-Power, 8-Bit DACs with 2-Wire Serial Interface in SOT23
//
//	https://datasheets.maximintegrated.com/en/ds/MAX5380-MAX5382.pdf

#define PS_BOARD_COUNT	2

typedef __packed struct {
	uint8_t MAX5380_Address;
} PS_Board_t;

PS_Board_t PS_Boards[PS_BOARD_COUNT];

/****************************************************************
 *
 *	Return PS board connection status
 *
 */
bool PS_IsConnect(uint8_t device)
{
	if (device < PS_BOARD_COUNT)
		return (PS_Boards[device].MAX5380_Address != 0);
	return false;
}

bool PS_IsNotConnect(uint8_t device)
{
	if (device < PS_BOARD_COUNT)
		return (PS_Boards[device].MAX5380_Address == 0);
	return true;
}

static bool MAX5380_SetEx(uint8_t device, uint8_t value, uint8_t shutdown)
{
	GPIO_TypeDef * scl_port;
	uint16_t scl_pin;
	uint8_t result = false;

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

	device = PS_Boards[device].MAX5380_Address;
	if (device == 0)
		return result;
	device |= shutdown;

	i2c_reset(scl_port, scl_pin);
	i2c_start(scl_port, scl_pin);

	if (i2c_write(scl_port, scl_pin, device)
	&&	i2c_write(scl_port, scl_pin, value)
		)
		result = true;

	i2c_stop(scl_port, scl_pin);

	return result;
}

/****************************************************************
 *
 *	Shutdown MAX5380
 *
 */
bool MAX5380_Shutdown(uint8_t device)
{
	if (PS_IsConnect(device))
		return MAX5380_SetEx(device, 255,  MAX5380_SHDN);
	return false;
}

/****************************************************************
 *
 *	Set value MAX5380
 *	Currectly separate SCLK for MAX5380
 *	TODO: Change to general SCL signal
 */
bool MAX5380_Set(uint8_t device, uint8_t value)
{
	if (PS_IsConnect(device))
		return MAX5380_SetEx(device, value, 0);
	return false;
}

/****************************************************************
 *
 *	Initialize MAX5380
 *
 */
const uint8_t MAX5380x[] = {
	MAX5380L,
	MAX5380M,
	MAX5380N,
	MAX5380P,
	0
};

void MAX5380_Init(void)
{
	uint8_t device;
	const uint8_t * address;

	for (device = 0; device < PS_BOARD_COUNT; device++)
	{
		address = MAX5380x;
		for(;;)
		{
			PS_Boards[device].MAX5380_Address = *address;
			if (*address == 0 || MAX5380_Shutdown(device))
				break;
			address++;
		}
	}
}
