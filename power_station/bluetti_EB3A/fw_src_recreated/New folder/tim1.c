// tim1.c
#include "tim1.h"

// Примерные структуры, отражающие layout param_2/param_1 из оригинала:
typedef struct {
    uint16_t prescaler;
    uint16_t unused0;
    uint32_t arr;
    uint16_t cr1_bits;    // комбинация MODE, CMS, DIR, etc.
    uint16_t cr1_pres;
    uint8_t  repetition;  // для TIM1/TIM15–17
} TIM_BasicConfig;

typedef struct {
    uint8_t  oc_mode;     // OCxM, OCxPE
    uint8_t  unused1;
    uint8_t  polarity;    // CCxP, CCxNP
    uint8_t  idle_state;  // OISx, OISxN
    uint32_t pulse;       // CCRx
    // ... и ещё несколько полей для complementary
} TIM_ChannelConfig;

typedef struct {
    uint16_t dtg;   // Dead-time generator
    uint16_t lock;  // LOCK bits
    uint16_t ossr;  // Off-state selection run
    uint16_t ossi;  // Off-state selection idle
    uint16_t bke;   // Break enable
    uint16_t bkp;   // Break polarity
    uint16_t aom;   // Automatic output enable
} TIM_BreakDeadTimeConfig;

// Далее реализации, которые внутри будут копировать биты из cfg в регистры:
// e.g.:
void TIM1_ConfigureBase(const TIM_BasicConfig *c) {
    // CR1: MODE, CMS, DIR, вперемешку с c->cr1_bits
    TIM1->PSC = c->prescaler;
    TIM1->ARR = c->arr;
    TIM1->CR1 = (TIM1->CR1 & ~MASK) | c->cr1_bits;
    // Повторитель:
    TIM1->RCR = c->repetition;
    // Форс-обновление:
    TIM1->EGR = TIM_EGR_UG;
}
// и так далее для каждого из перечисленных выше методов...
