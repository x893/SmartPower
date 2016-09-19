#include "usart.h"
#include "main.h"

#include <stdbool.h>

typedef struct {
	uint8_t *	Buffer;
	int			Head;
	int			Tail;
	size_t		Size;
} RingBuffer_t;

uint8_t U1_Rx_Buffer[1];
uint8_t U2_Rx_Buffer[1];

uint8_t U1_Rx_DataBuffer[64];
uint8_t U2_Rx_DataBuffer[64];

RingBuffer_t U1_Rx_RingBuffer;
RingBuffer_t U2_Rx_RingBuffer;

void RingBuffer_Init(RingBuffer_t * ring, uint8_t * buffer, size_t size)
{
	ring->Size = 0;
	ring->Buffer = buffer;
	ring->Head = 0;
	ring->Tail = 0;
	ring->Size = size;
}

bool RingBuffer_Put(RingBuffer_t * ring, uint8_t data)
{
	int next, current;

	// Put disable
	if (ring->Size == 0)
		return false;

	// Check for index ring
	current = ring->Head;
	next = current + 1;
	if (next == ring->Size)
		next = 0;

	// Check for space in buffer
	if (next == ring->Tail)
		return false;
	
	ring->Buffer[current] = data;
	ring->Head = next;
	return true;
}

int RingBuffer_Available(RingBuffer_t * ring)
{
	int count = ring->Head - ring->Tail;

	if (count < 0)
		count += ring->Size;

	return count;
}

bool RingBuffer_Get(RingBuffer_t * ring, uint8_t * pData)
{
	int tail = ring->Tail;
	if (ring->Head == tail)
		return false;
	*pData = ring->Buffer[tail];
	tail++;
	if (tail == ring->Size)
		tail = 0;
	ring->Tail = tail;
	return true;
}

static void usart_start(UART_HandleTypeDef *huart, uint8_t * pData)
{
	HAL_UART_Receive_IT(huart, pData, 1);
}

void usart1_start(void)
{
	RingBuffer_Init(&U1_Rx_RingBuffer, U1_Rx_DataBuffer, sizeof(U1_Rx_DataBuffer));
	HAL_UART_Receive_IT(&huart1, U1_Rx_Buffer, 1);
}

void usart2_start(void)
{
	RingBuffer_Init(&U2_Rx_RingBuffer, U2_Rx_DataBuffer, sizeof(U2_Rx_DataBuffer));
	HAL_UART_Receive_IT(&huart2, U2_Rx_Buffer, 1);
}

int usart1_available()
{
	return RingBuffer_Available(&U1_Rx_RingBuffer);
}

int usart2_available()
{
	return RingBuffer_Available(&U2_Rx_RingBuffer);
}

uint8_t usart1_read(uint8_t * pData)
{
	return RingBuffer_Get(&U1_Rx_RingBuffer, pData);
}

uint8_t usart2_read(uint8_t * pData)
{
	return RingBuffer_Get(&U2_Rx_RingBuffer, pData);
}

void usart1_send(uint8_t * pData, uint16_t size)
{
	HAL_UART_Transmit(&huart1, pData, size, size / 5 + 1);
}

void usart2_send(uint8_t * pData, uint16_t size)
{
	HAL_UART_Transmit(&huart2, pData, size, size / 5 + 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		RingBuffer_Put(&U2_Rx_RingBuffer, U2_Rx_Buffer[0]);
		usart_start(huart, U2_Rx_Buffer);
		return;
	}
	if (huart->Instance == USART1)
	{
		RingBuffer_Put(&U1_Rx_RingBuffer, U1_Rx_Buffer[0]);
		usart_start(huart, U1_Rx_Buffer);
		return;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		Context.Errors |= USART2_ERROR;
		usart_start(huart, U2_Rx_Buffer);
		return;
	}
	if (huart->Instance == USART1)
	{
		Context.Errors |= USART1_ERROR;
		usart_start(huart, U1_Rx_Buffer);
		return;
	}
}
