#ifndef GPIO_H
#define GPIO_H

#include "lib.h"

typedef struct
{
    volatile uint32_t Mode;
    volatile uint16_t Type;
    volatile uint16_t reserved1;
    volatile uint32_t Speed;
    volatile uint32_t Pull;
    volatile uint16_t IDR;
    volatile uint16_t reserved2;
    volatile uint16_t ODR;
    volatile uint16_t reserved3;
    volatile uint32_t BSRR;
    volatile uint16_t LCKR;
    volatile uint16_t reserved4;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint16_t BRR;
    volatile uint16_t reserved5;
    volatile uint16_t ASCR;
    volatile uint16_t reserved6;
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)0x48000000)
#define GPIOB ((GPIO_TypeDef *)0x48000400)
#define GPIOC ((GPIO_TypeDef *)0x48000800)
#define GPIOD ((GPIO_TypeDef *)0x48000C00)
#define GPIOE ((GPIO_TypeDef *)0x48001000)
#define GPIOF ((GPIO_TypeDef *)0x48001400)
#define GPIOG ((GPIO_TypeDef *)0x48001800)
#define GPIOH ((GPIO_TypeDef *)0x48001C00)

#define GPIOAEN 0x01
#define GPIOBEN 0x02
#define GPIOCEN 0x04
#define GPIODEN 0x08
#define GPIOEEN 0x10
#define GPIOFEN 0x20
#define GPIOGEN 0x40

#define GPIO_PIN_0 0x0001
#define GPIO_PIN_1 0x0002
#define GPIO_PIN_2 0x0004
#define GPIO_PIN_3 0x0008
#define GPIO_PIN_4 0x0010
#define GPIO_PIN_5 0x0020
#define GPIO_PIN_6 0x0040
#define GPIO_PIN_7 0x0080
#define GPIO_PIN_8 0x0100
#define GPIO_PIN_9 0x0200
#define GPIO_PIN_10 0x0400
#define GPIO_PIN_11 0x0800
#define GPIO_PIN_12 0x1000
#define GPIO_PIN_13 0x2000
#define GPIO_PIN_14 0x4000
#define GPIO_PIN_15 0x8000

typedef enum
{
    AF0 = 0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15
} AF;

// For GPIOG
#define PWR 0x40007000
#define PWR_CR2 0x004
#define IOSV 9

typedef enum
{
    RESET = 0,
    SET
} OPERATION;

typedef enum
{
    MODE_INPUT = 0,
    MODE_OUTPUT,
    MODE_ALTERNATE,
    MODE_ANALOG
} MODETYPES;

typedef enum
{
    LOW_SPEED = 0,
    MEDIUM_SPEED,
    HIGH_SPEED,
    HIGH_HIGH_SPEED
} SPEEDTYPES;

typedef enum
{
    PULL_NO_PUPD = 0,
    PULL_PU,
    PULL_PD,
} PULLTYPES;

void gpioInitialization(GPIO_TypeDef *port, MODETYPES mode, SPEEDTYPES speed, PULLTYPES pull, AF af, uint32_t pins)
{
    for (int i = 0; i < 16; i++)
    {
        if (pins & (1 << i))
        {
            if (i < 8)
            {
                port->AFRL &= ~(0b1111 << (i * 4)); // Clear alternate function bits
                port->AFRL |= (af << (i * 4));      // Set alternate function
            }
            else
            {
                port->AFRH &= ~(0b1111 << ((i - 8) * 4)); // Clear alternate function bits
                port->AFRH |= (af << ((i - 8) * 4));      // Set alternate function
            }
            port->Mode &= ~(0b11 << (i * 2)); // Clear mode bits
            port->Mode |= (mode << (i * 2));  // Set mode

            port->Speed &= ~(0b11 << (i * 2)); // Clear speed bits
            port->Speed |= (speed << (i * 2)); // Set speed

            port->Pull &= ~(0b11 << (i * 2)); // Clear pull bits
            port->Pull |= (pull << (i * 2));  // Set pull
        }
    }
}

void gpioWrite(GPIO_TypeDef *port, uint32_t pin, OPERATION op)
{
    if (op == RESET)
    {
        REG_SET(port->BRR, pin);
    }
    else if (op == SET)
    {
        REG_SET(port->BSRR, pin);
    }
    return;
}

uint32_t gpioRead(GPIO_TypeDef *port, uint32_t pin)
{
    return (REG_CHECK(port->IDR, pin));
}

void gpioGivePowerToGPIOG(void)
{
    volatile uint32_t *pwr_cr2 = (uint32_t *)(PWR + PWR_CR2);
    BIT_SET(*pwr_cr2, IOSV);
    return;
}

#endif
