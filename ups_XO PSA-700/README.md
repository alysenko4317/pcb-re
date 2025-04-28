# Reverse-Engineering the PSA-700 Inverter Board

Welcome to the behind-the-scenes of my attempts to bring this charger’s inverter back to life — so far, more like pushing it into a coma than reviving it, but I did learn a thing or two.

---

## The Beginning: From Photo to Schematic

I started by photographing both sides of the PCB using a twenty-year-old Canon IXUS in macro mode (I’m no pro photographer, so the shots aren’t perfect, but they got the job done), then painstakingly aligned the layers in Photoshop. From there, I traced every copper pours and vias in Sprint-Layout, recreating:

- The **power stage** with MOSFET bridge and gate drives  
- The **5 V regulator** and auxiliary circuits  
- The **fault detection** network (over-current, shoot-through comparators)  
- The **shutdown behavior**: the inverter would run for about 2 seconds then shut itself off—exact firmware logic unknown, but the power stage clearly passed initial self-tests  

I selectively highlighted and annotated key nets—enough to transform the board’s jumble of tracks into a working schematic foundation.

---

## Protocol Detective Work

Next up was traffic analysis:

- Sniffed the UART link between the main MCU and the auxiliary 8-bit CHIPSEA MCU in the HV section—discovered 10-byte frames at 2400 baud with an XOR-based CRC.
- Observed I²C communication between the inverter board and the station’s main controller (which drives the display module and buttons). Over this interface, the inverter receives on/off commands and reports status, load, etc. The protocol proved quite complex, so I decided not to dig into it further.

This step felt like eavesdropping on the board’s own nervous system.

---

## Lessons Learned & Mishaps

- **Fried the main MCU while probing**  
  I soldered wires directly to the optoisolator inputs to measure switching frequency and dead-time. During soldering, the loose end accidentally touched a nearby capacitor, shorted the power rail, and killed the MCU. That surge also took out one of the MOSFETs and damaged an optoisolator input. Next time I’ll use spring-clip probes instead of soldering onto live traces.

Each failure taught me a new trick and made me more patient.

---

_This README focuses solely on reverse-engineering. All MCU firmware and board mods now live in the separate STM32-Inverter repository._

> Check out my **STM32-Inverter** portfolio for the next chapter:  
> https://github.com/alysenko4317/STM32-Inverter/tree/main/Inverter_F030_PSA
