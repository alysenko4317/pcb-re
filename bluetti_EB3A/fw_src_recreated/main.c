#include "ll_utils.h"
#include "system_clock_utils.h"
#include "peripherals_init.h"
#include "iwdg_utils.h"

#define MAGIC_BOOTLOADER_VALUE  0x1F000000U
#define MAGIC_BOOTLOADER_ADDR   ((volatile uint32_t *)0x00000004U)

// Placeholders
void InitSystemTimer(void);
void InitUart1WithDma(void);
void InitDescriptorTable(uint32_t base);
void CheckAndInitSpecialMode(void);
void ProcessCommPacket(void);

// Bootloader entry point (optional)
void BootloaderEntry(void);

// Forward declarations
void FirmwareMainEntry(void);
void MainLoop(void);

int main(void)
{
    // Simulate Reset_Handler logic
    bool isPrivileged = true;  // if needed — skip for now

    // Check magic bootloader value
    if ((*MAGIC_BOOTLOADER_ADDR >> 24) == 0x1FU)
    {
        // Enable clock for SYSCFG (bit SYSCFGEN = 1)
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        // Reset SYSCFG_CFGR1 to default (disable all remaps etc.)
        SYSCFG->CFGR1 = 0;
    }

    SystemInit_RCC();
    FirmwareMainEntry();

    while (1) {}
}

void FirmwareMainEntry(void)
{
    MainLoop();
}

void MainLoop(void)
{
    InitPeripheralClocks();
    IWDG_Init();
    InitSystemTimer();
    InitUart1WithDma();

    InitDescriptorTable(0x20000000);
    CheckAndInitSpecialMode();

    // Example init for testing LEDs
    InitLeds();

    while (1)
    {
        // Simple LED blink test (optional)
        GPIOA->ODR ^= (1U << 0);
        for (volatile int i = 0; i < 100000; i++);

        ProcessCommPacket();
        IWDG_Refresh();

        // Example: if button pressed → enter bootloader
        if (IsButtonPressed())
        {
            EnterBootloaderMode();
        }
    }
}

// Dummy LED init (replace with your GPIO pins)
void InitLeds(void)
{
    GPIO_Config led_config = {
        .PinMask = (1U << 0),  // e.g. PA0
        .Mode = GPIO_MODE_OUTPUT,
        .Pull = GPIO_NOPULL,
        .Alternate = 0,
        .Speed = GPIO_SPEED_HIGH
    };

    GPIO_ConfigurePins(GPIOA, &led_config);
}

bool IsButtonPressed(void)
{
    // Example: PA1 as button input
    return ((GPIOA->IDR & (1U << 1)) == 0);
}

void EnterBootloaderMode(void)
{
    __disable_irq();

    *MAGIC_BOOTLOADER_ADDR = MAGIC_BOOTLOADER_VALUE;

    NVIC_SystemReset();
}
