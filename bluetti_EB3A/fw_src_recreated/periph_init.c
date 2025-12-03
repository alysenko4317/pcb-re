void InitPeripheralClocks(void)
{
    /* This is a very typical peripheral clock setup for an inverter or
       power management device:
       
       GPIO → control LEDs, modes
       USART → interfaces (MODBUS, BLE, USB-UART bridge)
       DMA → optimize data transfer
       TIM → PWM generation, frequency counters, timing control
       SYSCFG → remap, EXTI lines
       COMP → e.g. zero-cross detect or overcurrent protection */

    // Enable APB2 peripheral clocks:
    // SYSCFG + COMP | USART1 | TIM16 | TIM17
    RCC_EnableClock_APB2(0x1E0000, true);

    // Enable APB1 peripheral clocks:
    // USART2 | USART3
    RCC_EnableClock_APB1(0x60000, true);

    // Enable AHB peripheral clocks:
    // DMA
    RCC_EnableClock_AHB(0x4000, true);

    // GPIOC
    RCC_EnableClock_AHB(0x40, true);

    // GPIOA
    RCC_EnableClock_AHB(0x1, true);
}
