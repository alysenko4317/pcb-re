extern unsigned long _estack; // top of stack (from linker script)

__attribute__ ((section(".isr_vector")))
const void *VectorTable[] =
{
    &_estack,                 // Initial stack pointer
    Reset_Handler,            // Reset Handler
    NMI_Handler,              // NMI Handler
    HardFault_Handler,        // Hard Fault Handler
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    SVC_Handler,              // SVCall Handler
    0,                        // Reserved
    0,                        // Reserved
    PendSV_Handler,           // PendSV Handler
    SysTick_Handler,          // SysTick Handler

    // External interrupts start here
    0,                        // IRQ0
    0,                        // IRQ1
    DMA1_Channel1_IRQHandler, // IRQ2 - DMA Channel 1
    0,                        // IRQ3
    0,                        // IRQ4
    USART1_IRQHandler,        // IRQ5 - USART1
    USART2_IRQHandler,        // IRQ6 - USART2
    Default_Handler,          // IRQ7
    Default_Handler,          // IRQ8
    // ... continue as needed
};
