#include "system_clock_utils.h"

/* SystemInit_RCC() — typical RCC initialization at startup.
   For STM32F0, this is normally called from Reset_Handler or early main,
   to bring RCC to a known state and configure system clocks. */
void SystemInit_RCC(void)
{
    // Enable HSI (bit 0), clear PLLON / CSSON / HSEON (bits 16,19,18)
    RCC->CR = (RCC->CR & 0xFEF2FFFFU) | RCC_CR_HSION;

    // Reset prescalers and clock sources (keep reserved bits)
    RCC->CFGR = RCC->CFGR & 0x08C0B80CU;

    // Reset PREDIV prescaler config
    RCC->CFGR2 = RCC->CFGR2 & 0xFFFFFFF0U;

    // Disable MCO output, reset USART/CEC/I2C clock settings
    RCC->CFGR3 = RCC->CFGR3 & 0xFFFFFEACU;

    // Disable HSI14 oscillator
    RCC->CR2 = RCC->CR2 & 0xFFFFFFFEU;

    // Disable all RCC clock interrupts and clear flags
    RCC->CIR = 0;

    // Setup PLL as system clock source
    SystemInit_PLL();
}

/* SystemInit_PLL() — standard function for switching SYSCLK to PLL.
   No unusual behavior here — straightforward clock setup. */
void SystemInit_PLL(void)
{
    int timeout_counter = 0;
    uint32_t rcc_cr_value;

    // Wait until HSI is ready (HSIRDY bit)
    do
    {
        timeout_counter++;
        if (RCC->CR & RCC_CR_HSIRDY)
            break;
    } while (timeout_counter != 0x5000);

    rcc_cr_value = RCC->CR | RCC_CR_PLLON;

    // Check if PLL is not already locked
    if ((int)((RCC->CR | RCC_CR_PLLON) << 14) < 0)
    {
        // Set Flash latency = 1 wait state + enable prefetch buffer
        // and wait until PLL ready bit is cleared
        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

        // Wait until PLLRDY is cleared (PLL not locked yet)
        do
        {
            // Waiting for PLLRDY = 0
        } while ((int)((RCC->CR | RCC_CR_PLLON | RCC_CR_PLLRDY) << 6) >= 0);

        // Configure PLL as SYSCLK source:
        // - Set PLLSRC, PLLMUL bits
        // - Select PLL as system clock source
        RCC->CFGR = (RCC->CFGR & 0xFFC07FFCU) | 0x00110002U;

        // Wait until SWS bits indicate PLL is used as SYSCLK
        do
        {
            rcc_cr_value = RCC->CR | RCC_CR_PLLON | RCC_CR_PLLRDY;
        } while (((RCC->CFGR >> 2) & 0x3U) != 0x2U);
    }

    // Write back updated RCC.CR value (with PLLON set)
    RCC->CR = rcc_cr_value;
}
