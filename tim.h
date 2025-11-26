#ifndef TIM_H
#define TIM_H

#include "lib.h"

typedef struct {
    volatile uint16_t CR1;
	volatile uint16_t CR1_RESERVED;
	volatile uint16_t CR2;
	volatile uint16_t CR2_RESERVED;
	volatile uint32_t RESERVED1;
	volatile uint16_t DIER;
	volatile uint16_t DIER_RESERVED;
	volatile uint16_t SR;
	volatile uint16_t SR_RESERVED;
	volatile uint16_t EGR;
	volatile uint16_t EGR_RESERVED;
	volatile uint32_t RESERVED2;
	volatile uint32_t RESERVED3;
	volatile uint32_t RESERVED4;
	volatile uint32_t CNT;
	volatile uint16_t PSC;
	volatile uint16_t PSC_RESERVED;
	volatile uint16_t ARR;
	volatile uint16_t ARR_RESERVED;
} TIM_TypeDef;

#define TIM6 ((TIM_TypeDef *)0x40001000)
#define TIM6EN (0x1 << 4)

void timInitialization(TIM_TypeDef *tim, uint32_t psc, uint32_t arr)
{
	BIT_CLR(tim->SR, 0);
    REG_SET(tim->PSC, psc); // Set prescaler
    REG_SET(tim->ARR, arr); // Set auto-reload value
    BIT_SET(tim->DIER, 0);
    BIT_CLR(tim->CR1, 0);
    BIT_CLR(tim->CR1, 1);
    BIT_CLR(tim->CR1, 2);
    BIT_CLR(tim->CR1, 3);
    BIT_CLR(tim->CR1, 7);
    BIT_CLR(tim->CR1, 11);
    BIT_CLR(tim->SR, 0);
}

void timEnable(TIM_TypeDef *tim)
{
	BIT_SET(tim->CR1, 0); // Enable timer
}

int tim_check_flag(TIM_TypeDef *tim){
	return (BIT_CHECK(tim->SR, 0));
}

void tim_reset_flag(TIM_TypeDef *tim){
    BIT_CLR(tim->SR, 0);
}


#endif
