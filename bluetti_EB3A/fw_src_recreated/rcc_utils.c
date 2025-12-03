#include "rcc_utils.h"

void RCC_APB1PeripheralReset(uint32_t bitmask, bool assert_reset)
{
    if (assert_reset)
        RCC->APB1RSTR |= bitmask;
    else
        RCC->APB1RSTR &= ~bitmask;
}

void RCC_APB2PeripheralReset(uint32_t bitmask, bool assert_reset)
{
    if (assert_reset)
        RCC->APB2RSTR |= bitmask;
    else
        RCC->APB2RSTR &= ~bitmask;
}


void RCC_EnableClock_AHB(uint32_t bitmask, bool enable)
{
    if (enable)
        RCC->AHBENR |= bitmask;
    else
        RCC->AHBENR &= ~bitmask;
}

void RCC_EnableClock_APB1(uint32_t bitmask, bool enable)
{
    if (enable)
        RCC->APB1ENR |= bitmask;
    else
        RCC->APB1ENR &= ~bitmask;
}

void RCC_EnableClock_APB2(uint32_t bitmask, bool enable)
{
    if (enable)
        RCC->APB2ENR |= bitmask;
    else
        RCC->APB2ENR &= ~bitmask;
}

