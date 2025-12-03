#include "usart_utils.h"

void USART_Reset(USART_TypeDef *USARTx)
{
    if (USARTx == USART1)
    {
        RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
        RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;
    }
    else if (USARTx == USART2)
    {
        RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST;
        RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;
    }
    // Add other USARTs if needed
}

void USART_Configure(USART_TypeDef *USARTx, const USART_Config *config)
{
    USARTx->CR1 &= ~USART_CR1_UE;  // Disable USART

    USARTx->CR1 = config->CR1;
    USARTx->CR2 = config->CR2;
    USARTx->CR3 = config->CR3;

    USARTx->BRR = config->BaudrateReg;

    // Do not enable yet - controlled by SetEnabled
}

void USART_SetEnabled(USART_TypeDef *USARTx, bool enabled)
{
    if (enabled)
        USARTx->CR1 |= USART_CR1_UE;
    else
        USARTx->CR1 &= ~USART_CR1_UE;
}

void USART_EnableFeature(USART_TypeDef *USARTx, uint32_t feature_mask, bool enable)
{
    if (enable)
        USARTx->CR1 |= feature_mask;
    else
        USARTx->CR1 &= ~feature_mask;
}

void USART_EnableFeature_CR3(USART_TypeDef *USARTx, uint32_t feature_mask, bool enable)
{
    if (enable)
        USARTx->CR3 |= feature_mask;
    else
        USARTx->CR3 &= ~feature_mask;
}

void USART_ClearFlag(USART_TypeDef *USARTx, USART_ClearFlag flag)
{
    USARTx->ICR = flag;
}
