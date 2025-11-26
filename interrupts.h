#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "lib.h"

#define TIM6_DAC_IRQn 54
#define LPUART1_IRQn 70
#define NVIC_ISER_BASE 0xE000E100UL

void nvic_enable_irq(uint8_t irq_num) {
    volatile uint32_t* iser = (volatile uint32_t*)(NVIC_ISER_BASE + ((irq_num / 32) * 4));
    *iser = (1 << (irq_num % 32));
}

#endif
