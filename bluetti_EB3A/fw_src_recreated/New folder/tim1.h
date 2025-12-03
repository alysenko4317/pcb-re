// tim1.h
#pragma once
#include <stdint.h>
#include "peripherals.h"

// Базовая конфигурация таймера
void TIM1_ConfigureBase(const TIM_BasicConfig *cfg);
// Настройка PWM каналов
void TIM1_ConfigureChannel1PWM(const TIM_ChannelConfig *cfg);
void TIM1_EnableChannel1(bool enable);
void TIM1_ConfigureChannel2PWM(const TIM_ChannelConfig *cfg);
void TIM1_EnableChannel2(bool enable);
void TIM1_ConfigureChannel3PWM(const TIM_ChannelConfig *cfg);
void TIM1_EnableChannel3(bool enable);
// Настройка break/dead-time
void TIM1_ConfigureBreakDeadTime(const TIM_BreakDeadTimeConfig *cfg);
// Прерывания
void TIM1_EnableInterrupt(uint16_t irq_mask, bool enable);
// Запуск PWM
void TIM1_SetMainOutputEnable(bool enable);
void TIM1_EnableCounter(bool enable);
void TIM1_TriggerUpdateEvent(bool generate);
