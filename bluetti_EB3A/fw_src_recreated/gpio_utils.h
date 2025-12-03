#pragma once

#include "stm32f0xx.h"  // или твой дефайн

typedef struct {
    uint32_t PinMask;
    uint8_t Mode;      // GPIO_MODE_INPUT, OUTPUT, AF, ANALOG
    uint8_t Pull;      // GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
    uint8_t Alternate; // AF number
    uint8_t Speed;     // GPIO_SPEED_LOW/HIGH
} GPIO_Config;

void GPIO_ConfigurePins(GPIO_TypeDef *GPIOx, const GPIO_Config *config);
void GPIO_SetAlternateFunction(GPIO_TypeDef *GPIOx, uint32_t pin_number, uint32_t af_num);
