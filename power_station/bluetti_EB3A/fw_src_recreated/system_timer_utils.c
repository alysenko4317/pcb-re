void InitSystemTimer(void)
{
    uint32_t sysclk_freq;
    sysclk_freq = SystemCoreClock;  // или вручную → аналогично FUN_080010d0()

    uint32_t reload_value = sysclk_freq / 1000;  // For 1ms tick

    if (reload_value < 0x1000000)
    {
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
        SysTick->LOAD = reload_value - 1;
    }
}


void InitSystemTimer(void)
{
    uint32_t freq_table[10];
    ComputeBusFrequencies(freq_table);

    uint32_t sysclk_hz = freq_table[0];
    uint32_t reload_value = sysclk_hz / 1000U;  // For 1ms tick

    if (reload_value < 0x1000000U)
    {
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
        SysTick->LOAD = reload_value - 1U;
    }
}

#include "stm32f0xx.h"
#include "system_timer_utils.h"

void InitSystemTimer(void)
{
    uint32_t freq_table[10];
    ComputeBusFrequencies(freq_table);

    uint32_t sysclk_hz = freq_table[0];

    // Option 1: Use simple division (accurate enough for SysTick)
    uint32_t reload_value = sysclk_hz / 1000U;

    // Option 2: Use ComputeScalingFactor if you want 100% behavior match
    // uint32_t reload_value = ComputeScalingFactor(sysclk_hz, 1000U);

    // Check if reload value fits in SysTick (24-bit counter)
    if (reload_value < 0x1000000U)
    {
        // Disable SysTick during setup
        SysTick->CTRL = 0;

        // Set reload value
        SysTick->LOAD = reload_value - 1U;

        // Reset current value to 0
        SysTick->VAL = 0;

        // Configure SysTick:
        // - Processor clock (CLKSOURCE)
        // - Enable SysTick interrupt (TICKINT)
        // - Enable SysTick counter (ENABLE)
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    }
}



#include <stdint.h>

uint32_t ComputeScalingFactor(uint32_t freq_in_hz, uint32_t target_tick_hz)
{
    uint32_t reload = 0;
    uint32_t quotient = 0;
    uint32_t remainder = freq_in_hz;
    uint32_t divider = target_tick_hz;
    uint32_t bit = 0;

    // Early exit: divider too big
    if (divider == 0 || freq_in_hz < divider)
        return 0;

    // Align divider to highest bit <= remainder
    while ((divider << 1) <= remainder && bit < 24)
    {
        divider <<= 1;
        bit++;
    }

    // "Shift-and-subtract" division
    while (bit < 32)
    {
        quotient <<= 1;
        if (remainder >= divider)
        {
            remainder -= divider;
            quotient |= 1;
        }

        divider >>= 1;
        bit++;
    }

    // Clamp reload value to SysTick limit (24 bits)
    if (quotient > 0xFFFFFF)
        quotient = 0xFFFFFF;

    return quotient;
}

