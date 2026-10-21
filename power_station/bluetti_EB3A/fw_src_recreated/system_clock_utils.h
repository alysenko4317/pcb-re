#pragma once

#include "stm32f0xx.h"

// System clock initialization (called at early startup)
void SystemInit_RCC(void);

// Switch SYSCLK to PLL (used by SystemInit_RCC)
void SystemInit_PLL(void);
