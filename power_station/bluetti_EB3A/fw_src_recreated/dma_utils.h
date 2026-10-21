#pragma once

#include "stm32f0xx.h"

typedef struct {
    uint32_t CCR;
    uint32_t CNDTR;
    uint32_t CPAR;
    uint32_t CMAR;
} DMA_ChannelConfig;

void DMA_ResetChannel(DMA_Channel_TypeDef *DMA_Channel);
void DMA_ConfigureChannel(DMA_Channel_TypeDef *DMA_Channel, const DMA_ChannelConfig *config);
void DMA_ClearInterruptFlags(uint32_t flag_mask);
void DMA_EnableChannelFeature(DMA_Channel_TypeDef *DMA_Channel, uint32_t feature_mask, bool enable);
void DMA_SetChannelEnabled(DMA_Channel_TypeDef *DMA_Channel, bool enable);
void DMA_ConfigureMux(uint32_t DMAMUX_base, uint32_t config_word);
