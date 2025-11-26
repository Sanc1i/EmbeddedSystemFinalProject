#ifndef LED_H
#define LED_H

#include "lib.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "lpuart.h"

#define D0_PIN GPIO_PIN_6
#define D0_PORT GPIOC

#define D1_PIN GPIO_PIN_7
#define D1_PORT GPIOC

#define D2_PIN GPIO_PIN_8
#define D2_PORT GPIOC

#define D3_PIN GPIO_PIN_9
#define D3_PORT GPIOC

#define D4_PIN GPIO_PIN_4
#define D4_PORT GPIOE

#define D5_PIN GPIO_PIN_3
#define D5_PORT GPIOD

#define D6_PIN GPIO_PIN_5
#define D6_PORT GPIOE

#define D7_PIN GPIO_PIN_6
#define D7_PORT GPIOE

#define LED_RED_PIN GPIO_PIN_13
#define LED_RED_PORT GPIOD

#define LED_GREEN_PIN GPIO_PIN_8
#define LED_GREEN_PORT GPIOB

#define LED_BLUE_PIN GPIO_PIN_12
#define LED_BLUE_PORT GPIOD

#define JOY_RIGHT_PIN GPIO_PIN_0
#define JOY_RIGHT_PORT GPIOE

#define JOY_LEFT_PIN GPIO_PIN_1
#define JOY_LEFT_PORT GPIOE

#define JOY_DOWN_PIN GPIO_PIN_2
#define JOY_DOWN_PORT GPIOE

#define JOY_UP_PIN GPIO_PIN_3
#define JOY_UP_PORT GPIOE

#define JOY_OK_PIN GPIO_PIN_15
#define JOY_OK_PORT GPIOE

const uint8_t led_count = 8;
uint16_t ledpins[] = {D0_PIN, D1_PIN, D2_PIN, D3_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN};
GPIO_TypeDef* ledports[] = {D0_PORT, D1_PORT, D2_PORT, D3_PORT, D4_PORT, D5_PORT, D6_PORT, D7_PORT};

const uint8_t rgb_count = 3;
uint16_t rgbpins[] = {LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN};
GPIO_TypeDef* rgbports[] = {LED_RED_PORT, LED_GREEN_PORT, LED_BLUE_PORT};

const uint8_t joy_count = 5;
uint16_t joypins[] = {JOY_RIGHT_PIN, JOY_LEFT_PIN, JOY_DOWN_PIN, JOY_UP_PIN, JOY_OK_PIN};
GPIO_TypeDef* joyports[] = {JOY_RIGHT_PORT, JOY_LEFT_PORT, JOY_DOWN_PORT, JOY_UP_PORT, JOY_OK_PORT};

static int get_led_index(int id) {
    if (id >= 0 && id < led_count) return id;
    if (id == 'R') return 100; // Red
    if (id == 'G') return 101; // Green
    if (id == 'B') return 102; // Blue
    return -1;
}

void led_set(int id) {
    int idx = get_led_index(id);
    if (idx >= 0 && idx < led_count) {
        gpioWrite(ledports[idx], ledpins[idx], SET);
    } else if (idx == 100) {
        gpioWrite(rgbports[0], rgbpins[0], SET);
    } else if (idx == 101) {
        gpioWrite(rgbports[1], rgbpins[1], SET);
    } else if (idx == 102) {
        gpioWrite(rgbports[2], rgbpins[2], SET);
    }else{
    	sendString(LPUART1, "Invalid LED id\n\r");
    	return;
    }
}

void led_clear(int id) {
    int idx = get_led_index(id);
    if (idx >= 0 && idx < led_count) {
        gpioWrite(ledports[idx], ledpins[idx], RESET);
    } else if (idx == 100) {
        gpioWrite(rgbports[0], rgbpins[0], RESET);
    } else if (idx == 101) {
        gpioWrite(rgbports[1], rgbpins[1], RESET);
    } else if (idx == 102) {
        gpioWrite(rgbports[2], rgbpins[2], RESET);
    }else{
    	sendString(LPUART1, "Invalid LED id\n\r");
    	return;
    }
}

void led_toggle(int id) {
    int idx = get_led_index(id);
    if (idx >= 0 && idx < led_count) {
        // Read current state and toggle
        uint32_t state = gpioRead(ledports[idx], ledpins[idx]);
        gpioWrite(ledports[idx], ledpins[idx], state ? RESET : SET);
    } else if (idx == 100) {
        uint32_t state = gpioRead(rgbports[0], rgbpins[0]);
        gpioWrite(rgbports[0], rgbpins[0], state ? RESET : SET);
    } else if (idx == 101) {
        uint32_t state = gpioRead(rgbports[1], rgbpins[1]);
        gpioWrite(rgbports[1], rgbpins[1], state ? RESET : SET);
    } else if (idx == 102) {
        uint32_t state = gpioRead(rgbports[2], rgbpins[2]);
        gpioWrite(rgbports[2], rgbpins[2], state ? RESET : SET);
    }else{
    	sendString(LPUART1, "Invalid LED id\n\r");
    	return;
    }

}

void led_status(int id) {
    int idx = get_led_index(id);
    int state = -1;
    if (idx >= 0 && idx < led_count) {
        state = gpioRead(ledports[idx], ledpins[idx]);
    } else if (idx == 100) {
        state = gpioRead(rgbports[0], rgbpins[0]);
    } else if (idx == 101) {
        state = gpioRead(rgbports[1], rgbpins[1]);
    } else if (idx == 102) {
        state = gpioRead(rgbports[2], rgbpins[2]);
    } else{
    	sendString(LPUART1, "Invalid LED id\n\r");
    	return;
    }
    if (state == 0) {
        sendString(LPUART1, "LED is OFF\n\r");
    } else {
        sendString(LPUART1, "LED is ON\n\r");
    }
}

void joy_read(int id) {
    int idx = -1;
    char msg[64];
    switch (id) {
        case 'L': idx = 1; break; // Left
        case 'R': idx = 0; break; // Right
        case 'D': idx = 2; break; // Down
        case 'U': idx = 3; break; // Up
        case 'C': idx = 4; break; // Center (OK)
        default:
            sendString(LPUART1, "Invalid joystick id\n\r");
            return;
    }
    int state = gpioRead(joyports[idx], joypins[idx]);
    snprintf(msg, sizeof(msg), "Joystick %c is %s\n\r", id, state ? "RELEASED" : "PRESSED");
    sendString(LPUART1, msg);
}

#endif
