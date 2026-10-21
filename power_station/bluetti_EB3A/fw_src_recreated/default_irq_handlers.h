#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Declare weak default handler
void Default_Handler(void) __attribute__((noreturn));
void NMI_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)__attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)  __attribute__((weak, alias("Default_Handler")));

// Declare other IRQs as needed
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
// Add more as needed...

#ifdef __cplusplus
}
#endif
