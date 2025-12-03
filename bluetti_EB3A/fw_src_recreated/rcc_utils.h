#pragma once

#include "stm32f0xx.h"

void RCC_APB1PeripheralReset(uint32_t bitmask, bool assert_reset);
void RCC_APB2PeripheralReset(uint32_t bitmask, bool assert_reset);
void RCC_EnableClock_AHB(uint32_t bitmask, bool enable);
void RCC_EnableClock_APB1(uint32_t bitmask, bool enable);
void RCC_EnableClock_APB2(uint32_t bitmask, bool enable);
