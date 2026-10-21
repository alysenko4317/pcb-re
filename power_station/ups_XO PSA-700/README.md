# Reverse-Engineering the PSA-700 Inverter Board

This repository contains reverse-engineering material collected while diagnosing and attempting to repair the inverter board from a PSA-700 power station.

The repair was not completed, but a substantial part of the inverter circuitry was traced and several useful observations, measurements, and software tools were collected during the investigation.

## PCB Photos

Component side:

![PSA-700 inverter board — component side](<pcb/IMG_1070_5_FV_FH+micro (351-3-8).jpg>)

Back side:

![PSA-700 inverter board — back side](<pcb/IMG_1068_3-FV_FH+micro (351-3-3).jpg>)

## Schematic

The inverter schematic is available in two forms:

- `inv2.spl8` — the primary editable schematic in **sPlan 8** format;
- `inv2.svg` — an SVG export intended for convenient viewing directly from the repository.

The sPlan file is the main source, and the SVG version is intended to be kept synchronized with it.

![PSA-700 inverter schematic](inv2.svg)

The schematic was reconstructed manually from the PCB, so it should be treated as reverse-engineering work rather than a verified manufacturer schematic.

The traced circuitry includes the inverter power stage, gate-drive circuitry, auxiliary supplies, hardware protection circuitry, and the interfaces around the main MCU and the isolated high-voltage section.

## Repair History

The station arrived after an incomplete previous repair with the inverter still non-functional.

Replacing the **LM393** used in the protection circuitry allowed the inverter to start. After that repair, the power stage would ramp up normally, but after roughly a second the firmware would begin a similarly smooth shutdown sequence. No obvious hardware protection signal was observed during this behavior, which suggested that at least a large part of the power stage was operational but that some internal firmware condition was still not being satisfied.

Further measurements showed that the synchronous-rectifier drive disappeared earlier than the rest of the inverter control signals: the rectifier started, its PWM pulse width increased during startup, and then the MCU stopped driving it while the primary-side converter and sine-forming stage continued briefly before shutdown.

The investigation ended after the main MCU was accidentally damaged during probing. A temporary wire soldered for measurements had a loose end that touched the case of a capacitor charged to battery voltage. This damaged the controller and made further repair of the original board impractical.

Although the repair itself therefore ended unsuccessfully, the reverse-engineering work completed before that point is preserved in this repository.

## Communication and Software Tools

Some additional work was done around the controllers and communication interfaces used by the inverter.

### HC32F030 Flasher

`soft/HC32F030 Flasher`

A UART-based utility for working with the **HC32F030** family MCU used on the board. It is based on the MCU's serial bootloader/ISP interface and can be used for operations such as reading, writing, erasing, verifying, locking, and rebooting supported HC32 devices.

### CHIPSEA UART Reader

`soft/CHIPSEA UART Reader`

A small utility used to capture and inspect the UART communication between the main MCU and the auxiliary **CHIPSEA** MCU located in the isolated high-voltage section.

The observed link uses approximately **2400 baud**, 8N1, with **10-byte packets** transmitted about every 50 ms. The last byte is a checksum calculated as XOR of the first nine bytes followed by XOR with `0x55`.

The auxiliary MCU reports several measurements from the high-voltage section, including the HV ground/reference level, the approximately +400 V rail, current-sense information, and the auxiliary supply voltage, together with a status byte.

More detailed protocol notes and measurements are included in the repository.

## Architecture Notes

The inverter is a reversible design built around multiple actively switched power stages and controlled primarily by a single MCU. In particular, the same controller drives the primary converter, the MOSFET synchronous rectifier on the transformer secondary, and the output sine-forming stage.

One interesting aspect of this implementation is the synchronous rectifier. There does not appear to be a direct high-speed current-feedback signal from the isolated high-voltage section to the main MCU. Instead, measurements from that section are collected by the auxiliary CHIPSEA MCU and returned over the isolated UART link at roughly 50 ms intervals — far too slowly to participate directly in individual switching decisions at the approximately 25 kHz converter frequency.

This raises an interesting reverse-engineering question about how the firmware determines the correct synchronous-rectifier timing and how it detects loss of the expected operating conditions. The available measurements suggest that the MCU must rely primarily on predetermined switching relationships and slower indirect feedback rather than cycle-by-cycle current sensing.

That does not by itself imply that the design is incorrect — this is a production power station and the implementation evidently works in normal units — but it is an unusual and interesting part of the topology worth documenting.

## Status

The original board is no longer a practical repair target because the main MCU was damaged during the investigation. The repository therefore remains primarily a record of the reverse-engineering work: PCB photographs, reconstructed schematic, measurements, protocol analysis, and supporting utilities.

Follow-up work involving replacement control electronics is kept separately in the **STM32-Inverter** repository:

https://github.com/alysenko4317/STM32-Inverter/tree/main/Inverter_F030_PSA
