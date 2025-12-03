#pragma once

#include "stm32f0xx.h"

void NVIC_ConfigureIRQ(uint32_t irq_number, uint32_t priority, bool enable);
