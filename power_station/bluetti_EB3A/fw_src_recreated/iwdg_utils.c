#include "iwdg_utils.h"

#define IWDG_KR_KEY_ENABLE   0xCCCC
#define IWDG_KR_KEY_REFRESH  0xAAAA
#define IWDG_KR_KEY_UNLOCK   0x5555

void IWDG_Unlock(void)
{
    IWDG->KR = IWDG_KR_KEY_UNLOCK;
}

void IWDG_SetPrescaler(uint8_t prescaler)
{
    IWDG->PR = prescaler & 0x7;  // 3 bits valid
}

void IWDG_SetReload(uint16_t reload)
{
    IWDG->RLR = reload & 0x0FFF; // 12 bits valid
}

void IWDG_Refresh(void)
{
    IWDG->KR = IWDG_KR_KEY_REFRESH;
}

void IWDG_Start(void)
{
    IWDG->KR = IWDG_KR_KEY_ENABLE;
}

void IWDG_Init(void)
{
    IWDG_Unlock();
    IWDG_SetPrescaler(3);
    IWDG_SetReload(0x28);
    IWDG_Refresh();
    IWDG_Start();
}
