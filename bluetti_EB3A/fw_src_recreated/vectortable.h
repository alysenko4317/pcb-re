typedef void (*ISR_Handler)(void);

typedef struct {
    uint32_t *initial_sp_value;
    ISR_Handler Reset_Handler;
    ISR_Handler NMI_Handler;
    ISR_Handler HardFault_Handler;
    ISR_Handler MemManage_Handler;
    ISR_Handler BusFault_Handler;
    ISR_Handler UsageFault_Handler;
    ISR_Handler Reserved0[4];
    ISR_Handler SVC_Handler;
    ISR_Handler DebugMon_Handler;
    ISR_Handler Reserved1;
    ISR_Handler PendSV_Handler;
    ISR_Handler SysTick_Handler;
    ISR_Handler IRQ_Handlers[48]; // можешь увеличить если нужно
} VectorTable_t;
