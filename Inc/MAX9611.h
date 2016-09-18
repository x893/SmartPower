#ifndef __MAX5400_H__
#define __MAX5400_H__

#include <stdint.h>
#include "i2c.h"

#define MAX9611_DEV_00	0xE0
#define MAX9611_DEV_01	0xE6
#define MAX9611_DEV_10	0xF8
#define MAX9611_DEV_11	0xFE

#define MAX9611_READ	0x01

#define MAX9611_CSA_H	0x00
#define MAX9611_CSA_L	0x01

#define MAX9611_RS_H	0x02
#define MAX9611_RS_L	0x03

#define MAX9611_OUT_H	0x04
#define MAX9611_OUT_L	0x05

#define MAX9611_SET_H	0x06
#define MAX9611_SET_L	0x07

#define MAX9611_TEMP_H	0x08
#define MAX9611_TEMP_L	0x09

#define MAX9611_CR1		0x0A
#define MAX9611_CR1_MUX		0x07

/*
	000 Channel A: Read current-sense amplifier output from ADC, gain = 1x
	001 Channel A: Read current-sense amplifier output from ADC, gain = 4x
	010 Channel A: Read current-sense amplifier output from ADC, gain = 8x
	011 Channel B: Read average voltage of RS+ (input common-mode voltage) from ADC
	100 Channel C: Read voltage of OUT from ADC
	101 Channel D: Read voltage of SET from ADC
	110 Channel E: Read internal die temperature from ADC
	111 Read all channels in fast-read mode, sequentially every 2ms. Uses last gain setting.
*/
#define MAX9611_CR1_SHDN	0x08
/*
	Power-on state = 0
	0 = Normal operation
	1 = Shutdown mode
*/
#define MAX9611_CR1_LR		0x10
/*
	0 = Normal operation
	1 = Reset if comparator is latched due to MODE = 111. This bit is automatically reset after a 1 is written.
*/
#define MAX9611_CR1_MODE	0xE0
/*
	000 = Normal operation for op amp/comparator
	111 = Comparator mode. OUT remains low until CSA output > VSET for 1ms, OUT latches
			high for 50ms, then OUT autoretries by going low. The comparator has an internal
			±10mV hysteresis voltage to help with noise immunity. For MAX9612, the polarity is
			reversed.
	011 = Op-amp mode. OUT regulates pFET for 1ms at VSET, OUT latches high for 50ms,
			then OUT autoretries by going low. For MAX9612, the polarity is reversed.
*/

#define MAX9611_CR2		0x0B
#define MAX9611_CR2_RTIM	0x04
/*
	Watchdog retry delay time
	0 = 50ms
	1 = 10ms
*/
#define MAX9611_CR2_DTIM	0x08
/*
	Watchdog delay time
	0 = 1ms
	1 = 100µs
*/


#ifdef __cplusplus
	extern "C" {
#endif

void MAX9611_Init(uint8_t device);

#ifdef __cplusplus
	}
#endif
	 
#endif
