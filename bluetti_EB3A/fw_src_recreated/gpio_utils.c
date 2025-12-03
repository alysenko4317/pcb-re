#include "gpio_utils.h"

void GPIO_ConfigurePins(GPIO_TypeDef *GPIOx, const GPIO_Config *config)
{
    uint32_t pos;
    uint32_t pin_mask = config->PinMask;

    for (pos = 0; pos < 16; pos++)
    {
        uint32_t current_pin = (1U << pos);
        if (pin_mask & current_pin)
        {
            // Configure mode
            GPIOx->MODER &= ~(0x3U << (pos * 2));
            GPIOx->MODER |= ((config->Mode & 0x3U) << (pos * 2));

            // Configure pull-up/pull-down
            GPIOx->PUPDR &= ~(0x3U << (pos * 2));
            GPIOx->PUPDR |= ((config->Pull & 0x3U) << (pos * 2));

            // Configure speed
            GPIOx->OSPEEDR &= ~(0x3U << (pos * 2));
            GPIOx->OSPEEDR |= ((config->Speed & 0x3U) << (pos * 2));

            // Configure alternate function (if applicable)
            if (config->Mode == GPIO_MODE_AF)
            {
                if (pos < 8)
                {
                    GPIOx->AFR[0] &= ~(0xFU << (pos * 4));
                    GPIOx->AFR[0] |= ((config->Alternate & 0xFU) << (pos * 4));
                }
                else
                {
                    GPIOx->AFR[1] &= ~(0xFU << ((pos - 8) * 4));
                    GPIOx->AFR[1] |= ((config->Alternate & 0xFU) << ((pos - 8) * 4));
                }
            }
        }
    }
}

void GPIO_SetAlternateFunction(GPIO_TypeDef *GPIOx, uint32_t pin_number, uint32_t af_num)
{
    if (pin_number < 8)
    {
        GPIOx->AFR[0] &= ~(0xFU << (pin_number * 4));
        GPIOx->AFR[0] |= ((af_num & 0xFU) << (pin_number * 4));
    }
    else
    {
        GPIOx->AFR[1] &= ~(0xFU << ((pin_number - 8) * 4));
        GPIOx->AFR[1] |= ((af_num & 0xFU) << ((pin_number - 8) * 4));
    }
}
