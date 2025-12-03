#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// === Main functions ===

void ProcessCommPacket(void);

void StartFirmwareFromBuffer(uint32_t *vector_table);

void SYSCFG_SetRemap(uint32_t remap_value);

uint32_t FirmwareUpload_ParseBlock(uint8_t *upload_state, int param_2, int param_3);

uint16_t ComputeCRC16_CCITT(const uint8_t *data, uint32_t length);

void CopyRxDataToBuffer(void *dest, const uint8_t *src, uint32_t length);

// === Helper variables (extern) ===

extern volatile uint8_t g_ParamBlockBuffer;
extern volatile char    g_ProtocolMode;
extern volatile int     g_fwUpdateInProgress;
extern volatile int     g_fwUpdateState;

void ResetSpecialModeFlags(void);

#ifdef __cplusplus
}
#endif
