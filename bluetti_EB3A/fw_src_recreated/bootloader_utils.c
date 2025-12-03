#include "bootloader_utils.h"

void EnterBootloaderMode(void)
{
    __disable_irq();

    *MAGIC_BOOTLOADER_ADDR = MAGIC_BOOTLOADER_VALUE;

    NVIC_SystemReset();
}

bool IsBootloaderRequested(void)
{
    return ((*MAGIC_BOOTLOADER_ADDR >> 24) == 0x1FU);
}
