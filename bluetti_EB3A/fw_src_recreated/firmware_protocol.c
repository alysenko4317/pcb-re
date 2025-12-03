#include "firmware_protocol.h"
#include "your_platform_headers.h"  // Include your peripheral + RCC + UART etc.

void ProcessCommPacket(void)
{
    uint8_t rxBuffer[1032];
    uint16_t rxLength = ReadRxBuffer(rxBuffer, 0x406);

    uint8_t responseBuffer[200];
    uint32_t responseLength = 0;

    ParseProtocolPacket(rxBuffer, rxLength, responseBuffer, (uint16_t *)&responseLength);

    if ((responseLength & 0xFFFF) == 0)
    {
        if ((g_ParamBlockBuffer == 0x02) && (g_ProtocolMode == 'U' || g_ProtocolMode == 'f'))
        {
            if (CheckFirmwareUpdateModeReady() == 0)
            {
                int result = HandleFirmwareUpdateMode(rxBuffer, rxLength);
                if (result != 0)
                {
                    UartSendPacketDMA(&result, 1);
                }
            }
            else if (g_ProtocolMode == 'U')
            {
                g_ProtocolMode = 'w';
            }
        }

        if (g_ProtocolMode == 'w')
        {
            StartFirmwareFromBuffer((uint32_t *)&g_fwUploadBuffer);
        }
    }
    else if (_DAT_200001A4 == 2)
    {
        _DAT_200001A4 = (uint32_t)(-1);
        g_ParamBlockBuffer = 0x02;
        g_ProtocolMode = 'U';
        ResetSpecialModeFlags();
        UartSendPacketDMA(responseBuffer, responseLength & 0xFFFF);
    }
    else
    {
        UartSendPacketDMA(responseBuffer, responseLength & 0xFFFF);
    }
}

void StartFirmwareFromBuffer(uint32_t *vector_table)
{
    disableIRQinterrupts();

    if ((vector_table[0] & 0x2FFE0000) == 0x20000000)
    {
        void (*entry_point)(void) = (void (*)(void))vector_table[1];

        CopyRxDataToBuffer((void *)0x20000000, (const uint8_t *)entry_point, 0xC0);

        if (isCurrentModePrivileged())
        {
            setMainStackPointer(vector_table[0]);
        }

        RCC_EnableClock_APB2(1);
        SYSCFG_SetRemap(3);

        entry_point();
    }

    enableIRQinterrupts();
}

void SYSCFG_SetRemap(uint32_t remap_value)
{
    uint32_t cfgr1 = Peripherals::SYSCFG.CFGR1;
    Peripherals::SYSCFG.CFGR1 = (cfgr1 & ~0x3) | remap_value;
}

uint32_t FirmwareUpload_ParseBlock(uint8_t *upload_state, int param_2, int param_3)
{
    uint32_t block_index = *(uint8_t *)(param_2 + 1);
    if (block_index + *(uint8_t *)(param_2 + 2) != 0xFF)
    {
        return 0x15;  // Error
    }

    if (*upload_state != block_index)
    {
        if ((*upload_state - 1) == block_index)
        {
            return 6;  // Repeated block
        }
        return 0x15;  // Error
    }

    uint16_t crc = ComputeCRC16_CCITT((uint8_t *)(param_2 + 3), param_3);
    uint16_t crc_in_packet = *(uint8_t *)(param_2 + param_3 + 3) |
                             (*(uint8_t *)(param_2 + param_3 + 4) << 8);

    if (crc_in_packet == crc)
    {
        ComputeScalingFactor(param_3, 0x406);
        CopyRxDataToBuffer((void *)(*(uint32_t *)(upload_state + 8)),
                           (uint8_t *)(param_2 + 3),
                           param_3);
        *(uint16_t *)(upload_state + 6) = (uint16_t)param_3;
        upload_state[1] = 1;
        *upload_state += 1;
        upload_state[4] = 1;
        return 6;
    }

    return 0x15;
}

uint16_t ComputeCRC16_CCITT(const uint8_t *data, uint32_t length)
{
    uint16_t crc = 0;

    while (length--)
    {
        crc ^= (*data++) << 8;
        for (int i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

void CopyRxDataToBuffer(void *dest, const uint8_t *src, uint32_t length)
{
    uint8_t *d = (uint8_t *)dest;

    while (length--)
    {
        *d++ = *src++;
    }
}

void ResetSpecialModeFlags(void)
{
    g_fwUpdateInProgress = 0;
    g_fwUpdateState = 0;
}
