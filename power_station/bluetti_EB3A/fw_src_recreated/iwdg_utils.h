#pragma once

#include "stm32f0xx.h"

void IWDG_Unlock(void);
void IWDG_SetPrescaler(uint8_t prescaler);
void IWDG_SetReload(uint16_t reload);
void IWDG_Refresh(void);
void IWDG_Start(void);

void IWDG_Init(void);
