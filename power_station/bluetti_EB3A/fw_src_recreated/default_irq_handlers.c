#include "default_handlers.h"

void Default_Handler(void)
{
    while (1)
    {
        // Optionally add a breakpoint instruction for debug:
        __asm__ volatile("bkpt #0");
    }
}
