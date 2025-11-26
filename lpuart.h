#ifndef LPUART_H
#define LPUART_H

#include "lib.h"

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t RESERVED1;
	volatile uint32_t RESERVED2;
	volatile uint16_t RQR;
	volatile uint16_t RQR_RESERVED;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint16_t RDR;
	volatile uint16_t RDR_RESERVED;
	volatile uint16_t TDR;
	volatile uint16_t TDR_RESERVED;
} LPUART_TypeDef;

#define LPUART1 ((LPUART_TypeDef *)0x40008000)
#define LPUART1EN 0x01

#define UE 0
#define RE 2
#define TE 3
#define RXNEIE 5
#define TXEIE 7
#define PCE 10
#define M0 12
#define M1 28
#define STOP1 12
#define STOP2 13
#define DMAT 7
#define DMAR 6
#define MSBFIRST 19

#define RXNE 5
#define TXE 7

void lpuartInitialization(LPUART_TypeDef *lpuart, uint32_t baudrate)
{

	BIT_CLR(lpuart->CR1, UE);
	// Baudrate
	REG_SET(lpuart->BRR, (4000000 << 8) / baudrate); // calc
	// 8 data bits
	BIT_CLR(lpuart->CR1, M0);
	BIT_CLR(lpuart->CR1, M1);

	// 1 stop bit
	BIT_CLR(lpuart->CR2, STOP1);
	BIT_CLR(lpuart->CR2, STOP2);

	// No parity
	BIT_CLR(lpuart->CR1, PCE);

	BIT_CLR(lpuart->CR3, DMAR);
	BIT_CLR(lpuart->CR3, DMAT);

	BIT_SET(lpuart->CR1, TE);
	BIT_SET(lpuart->CR1, RE);

	BIT_SET(lpuart->CR1, RXNEIE);
	// BIT_SET(lpuart->CR1, TXEIE);
}

void lpuartEnable(LPUART_TypeDef *lpuart)
{
	BIT_SET(lpuart->CR1, UE);
}

void lpuartDisable(LPUART_TypeDef *lpuart)
{
	BIT_CLR(lpuart->CR1, UE);
}

void lpuartSend(LPUART_TypeDef *lpuart, uint8_t data)
{
	while (!(lpuart->ISR & (0x1 << TXEIE)));
	lpuart->TDR = data;
}

uint8_t lpuartReceive(LPUART_TypeDef *lpuart)
{
	while (!(lpuart->ISR & (0x1 << RXNEIE)));
	return (uint8_t)(lpuart->RDR);
}

void sendChar(LPUART_TypeDef *lpuart, char c)
{
	lpuartSend(lpuart, (uint8_t)c);
}

void sendString(LPUART_TypeDef *lpuart, const char *str)
{
	while (*str)
	{
		lpuartSend(lpuart, (uint8_t)*str++);
	}
}

char receiveChar(LPUART_TypeDef *lpuart)
{
	return (char)lpuartReceive(lpuart);
}

unsigned int getBaudrate(LPUART_TypeDef *lpuart){
	return (unsigned int)lpuart->BRR;
}
#endif
