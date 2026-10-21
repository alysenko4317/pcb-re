#include "nvic_utils.h"

void NVIC_ConfigureIRQ(uint32_t irq_number, uint32_t priority, bool enable)
{
    uint32_t ip_index = irq_number >> 2;
    uint32_t ip_shift = (irq_number & 0x3U) * 8;

    NVIC->IP[ip_index] = (NVIC->IP[ip_index] & ~(0xFFU << ip_shift)) |
                         ((priority & 0xFFU) << ip_shift);

    if (enable)
        NVIC->ISER[0] = (1U << (irq_number & 0x1F));
    else
        NVIC->ICER[0] = (1U << (irq_number & 0x1F));
}
