#include "dma_utils.h"

void DMA_ResetChannel(DMA_Channel_TypeDef *DMA_Channel)
{
    DMA_Channel->CCR &= ~DMA_CCR_EN;  // Disable channel
    DMA_Channel->CCR = 0;
    DMA_Channel->CNDTR = 0;
    DMA_Channel->CPAR = 0;
    DMA_Channel->CMAR = 0;

    // Clear interrupt flags
    uint32_t channel_index = ((uint32_t)DMA_Channel - (uint32_t)&DMA1_Channel1) / 0x14; // 0..6
    DMA1->IFCR = (0xF << (channel_index * 4));
}

void DMA_ConfigureChannel(DMA_Channel_TypeDef *DMA_Channel, const DMA_ChannelConfig *config)
{
    DMA_Channel->CCR = config->CCR;
    DMA_Channel->CNDTR = config->CNDTR;
    DMA_Channel->CPAR = config->CPAR;
    DMA_Channel->CMAR = config->CMAR;
}

void DMA_ClearInterruptFlags(uint32_t flag_mask)
{
    DMA1->IFCR = flag_mask;
}

void DMA_EnableChannelFeature(DMA_Channel_TypeDef *DMA_Channel, uint32_t feature_mask, bool enable)
{
    if (enable)
        DMA_Channel->CCR |= feature_mask;
    else
        DMA_Channel->CCR &= ~feature_mask;
}

void DMA_SetChannelEnabled(DMA_Channel_TypeDef *DMA_Channel, bool enable)
{
    if (enable)
        DMA_Channel->CCR |= DMA_CCR_EN;
    else
        DMA_Channel->CCR &= ~DMA_CCR_EN;
}

void DMA_ConfigureMux(uint32_t DMAMUX_base, uint32_t config_word)
{
    volatile uint32_t *mux_reg = (uint32_t *)(DMAMUX_base + 0xA8);

    uint32_t index = (config_word >> 28) & 0xF;
    mux_reg[0] &= ~(0xF << (index * 4));
    mux_reg[0] |= (config_word & 0xFFFFFFF);
}
