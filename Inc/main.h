#ifndef __MAIN_H__
#define __MAIN_H__
#ifdef __cplusplus
	extern "C" {
#endif

#include <stdint.h>

#define ADC_TIMEOUT		20
#define ADC_ERROR		0x0001
#define USART1_ERROR	0x0002
#define USART2_ERROR	0x0004

typedef struct {
	uint16_t ADC_Value;
	uint16_t ADC_Timeout;
	uint16_t Errors;
} Context_t;

extern Context_t Context;


void LED_ON(void);
void LED_OFF(void);

#ifdef __cplusplus
	}
#endif
#endif
