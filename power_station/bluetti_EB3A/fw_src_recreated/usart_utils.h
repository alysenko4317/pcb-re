#pragma once

#include "stm32f0xx.h"

typedef struct {
    uint32_t BaudrateReg;  // USART_BRR
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
} USART_Config;

typedef enum
{
    USART_CLEAR_PECF     = USART_ICR_PECF,
    USART_CLEAR_FECF     = USART_ICR_FECF,
    USART_CLEAR_NCF      = USART_ICR_NCF,
    USART_CLEAR_ORECF    = USART_ICR_ORECF,
    USART_CLEAR_IDLECF   = USART_ICR_IDLECF,
    // Add others if needed
} USART_ClearFlag;

void USART_Reset(USART_TypeDef *USARTx);
void USART_Configure(USART_TypeDef *USARTx, const USART_Config *config);
void USART_SetEnabled(USART_TypeDef *USARTx, bool enabled);
void USART_EnableFeature(USART_TypeDef *USARTx, uint32_t feature_mask, bool enable);
void USART_EnableFeature_CR3(USART_TypeDef *USARTx, uint32_t feature_mask, bool enable);
void USART_ClearFlag(USART_TypeDef *USARTx, USART_ClearFlag flag);
