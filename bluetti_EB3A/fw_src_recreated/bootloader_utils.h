#pragma once

#include "stm32f0xx.h"

#define MAGIC_BOOTLOADER_VALUE  0x1F000000U
#define MAGIC_BOOTLOADER_ADDR   ((volatile uint32_t *)0x00000004U)

void EnterBootloaderMode(void);
bool IsBootloaderRequested(void);
