#ifndef __USART_BARD_H__
#define __USART_BARD_H__

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

void usart1_start(void);
void usart2_start(void);

int usart1_available(void);
int usart2_available(void);

uint8_t usart1_read(uint8_t * pData);
uint8_t usart2_read(uint8_t * pData);

void usart1_send(uint8_t * pData, uint16_t size);
void usart2_send(uint8_t * pData, uint16_t size);
		
#ifdef __cplusplus
	}
#endif
#endif
